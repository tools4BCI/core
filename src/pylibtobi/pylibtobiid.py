# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, either version 3 of
# the License, or (at your option) any later version.

# It is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public License
# along with this file.  If not, see <http://www.gnu.org/licenses/>.

from xml.dom.minidom import Document
from xml.dom.minidom import parseString
from inspect import stack
from pylibtobicore import *

IDTYPES_FAMILY_UNDEF                = "undef"
IDTYPES_FAMILY_BIOSIG               = "biosig"
IDTYPES_FAMILY_CUSTOM               = "custom"

IDMESSAGE_VERSION                   = "0.0.2.1"
IDMESSAGE_ROOTNODE                  = "tobiid"
IDMESSAGE_VERSIONNODE               = "version"
IDMESSAGE_FRAMENODE                 = "frame"
IDMESSAGE_DESCRIPTIONNODE           = "description"
IDMESSAGE_FAMILYNODE                = "family"
IDMESSAGE_EVENTNODE                 = "event"
IDMESSAGE_TIMESTAMPNODE             = "timestamp"
IDMESSAGE_REFERENCENODE             = "reference"


def current_function():
    return stack()[1][3]

#
# IDAsServer
#

class IDAsServer:
    def __init__(self):
        pass

#
# IDAsClient
#

class IDAsClient(TCBlock):
    BlockThis   = 0
    BlockPrev   = 1
    BlockNext   = 2
    BlockAll    = 3

    def __init__(self):
        self.__queue = []

    def Add(self, message, updatefidx = False):
        self.__queue.append(message)
        if updatefidx:
            TCBlock.SetBlockIdx(self, message.GetBlockIdx())

    def Get(message, idftype, idevent, direction):
        if not message:
            raise TCException("iD message needs to be allocated", '%s.%s' % (self.__class__.__name__, current_function()))

        if direction != IDAsClient.BlockAll and not TCBlock.IsSetBlockIdx(self):
            raise TCException("Block number must be set for searching Prev/Next", '%s.%s' % (self.__class__.__name__, current_function()))

        if self.Size() == 0:
            return None

        t_blockidx = TCBlock.BlockIdxUnset
        fmatch, tmatch, ematch = False, False, False

        for i in range(len(self.__queue)):
            t_blockidx = self.__queue[i].GetBlockIdx()
            t_type = self.__queue[i].GetFamilyType()
            t_event = self.__queue[i].GetEvent()

            # Match frame
            if direction == IDAsClient.BlockThis:
                fmatch = (t_blockidx == TCBlock.GetBlockIdx(self))
            elif direction == IDAsClient.BlockPrev:
                fmatch = (t_blockidx > TCBlock.GetBlockIdx(self))
            elif direction == IDAsClient.BlockNext:
                fmatch = (t_blockidx < TCBlock.GetBlockIdx(self))
            else: # BlockAll
                fmatch = True


            # Match type
            if idftype == IDMessage.FamilyBiosig:
                tmatch = (idftype == t_type)
            else: # IDMessage.FamilyUndef
                tmatch = True

            # Match event
            if idevent == IDMessage.EventNull:
                ematch = True
            else:
                ematch = (idevent == t_event)

            if tmatch and ematch and fmatch:
                m = self.__queue.pop(i)
                return m

        return None

    def Size(self):
        return len(self.__queue)

    def Clear(self):
        self.__queue = []
        return self.Size()

    def Dump(self):
        for i in self.__queue:
            i.Dump()

#
# IDMessage
#

class IDMessage(TCBlock):

    FamilyUndef         = -1
    FamilyBiosig        = 0
    FamilyCustom        = 1

    EventNull           = -1

    TxtFamilyUndef      = "FamilyUndef"
    TxtFamilyBiosig     = "FamilyBiosig"
    TxtFamilyCustom     = "FamilyCustom"

    def __init__(self, other = None, familyType = None, event = None):

        self.__Init()
        if other:
            self.Copy(other)
        elif familyType and event:
            self.__familyType = familyType
            self.__event = event
            self.__description = "unset"

    def __Init(self):
        TCBlock.SetBlockIdx(self, -1)
        self.__familyType = IDMessage.FamilyUndef
        self.__event = IDMessage.EventNull

    def Copy(self, other):
        TCBlock.SetBlockIdx(self, other.GetBlockIdx())
        self.__event = other.GetEvent()
        self.__familyType = other.GetFamilyType()
        self.__description = other.GetDescription()

    def GetDescription(self):
        return self.__description

    def SetDescription(self, description):
        self.__description = description

    def GetFamily(self):
        if self.__familyType == IDMessage.FamilyBiosig:
            return IDTYPES_FAMILY_BIOSIG
        elif self.__familyType == IDMessage.FamilyCustom:
            return IDTYPES_FAMILY_CUSTOM

        return IDTYPES_FAMILY_UNDEF

    def SetFamilyType(self, ftype):
        if isinstance(ftype, int):
            if type < IDMessage.FamilyUndef or type > IDMessage.FamilyCustom:
                return False
            self.__familyType = ftype
            return True
        else:
            if ftype == IDMessage.TxtFamilyUndef:
                self.__familyType = IDMessage.FamilyUndef
            elif ftype == IDMessage.TxtFamilyBiosig:
                self.__familyType = IDMessage.FamilyBiosig
            elif ftype == IDMessage.TxtFamilyCustom:
                self.__familyType = IDMessage.FamilyCustom
            else:
                return False
            return True

    def GetFamilyType(self):
        return self.__familyType

    def SetEvent(self, idevent):
        self.__event = idevent

    def GetEvent(self):
        return self.__event

    def Dump(self):
        print "[IDMessage::Dump] TOBI iD message for frame %d [%s]" % (TCBlock.GetBlockIdx(self), self.GetDescription())
        print " + Event family  %d/%s" % (self.GetFamilyType(), self.GetFamily())
        print " + Event value   %d" % (self.GetEvent())

    @staticmethod
    def FamilyType(family):
        if family == IDTYPES_FAMILY_BIOSIG:
            return IDMessage.FamilyBiosig
        else:
            return IDMessage.FamilyUndef

#
# IDSerializer
#

class IDSerializer:

    def __init__(self, message = None, indent = False, declaration = False):
        self.__message = message or None
        self.__indent = indent
        self.__declaration = declaration

    def SeMessage(self, message):
        self.__message = message

    def Serialize(self):
        if not self.__message:
            raise TCException("iD message not sent, cannot serialize", '%s.%s' % (self.__class__.__name__, current_function()))

        doc = Document()

        # TODO declaration bit

        cacheFidx = '%d' % self.__message.GetBlockIdx()
        cacheEvent = '%d' % self.__message.GetEvent()
        fvalue = self.__message.GetFamily()

        timestamp = self.__message.absolute.Get()
        reference = self.__message.reference.Get()

        root = doc.createElement(IDMESSAGE_ROOTNODE)
        root.setAttribute(IDMESSAGE_VERSIONNODE, IDMESSAGE_VERSION)
        root.setAttribute(IDMESSAGE_DESCRIPTIONNODE, self.__message.GetDescription())
        root.setAttribute(IDMESSAGE_FRAMENODE, cacheFidx)
        root.setAttribute(IDMESSAGE_FAMILYNODE, fvalue)
        root.setAttribute(IDMESSAGE_EVENTNODE, cacheEvent)
        root.setAttribute(IDMESSAGE_TIMESTAMPNODE, timestamp)
        root.setAttribute(IDMESSAGE_REFERENCENODE, reference)
        doc.appendChild(root)

        # TODO indent?

        return doc.toxml()

    def Deserialize(self, msg):
        try:
            doc = parseString(msg)
        except:
            raise TCException("iD root note not found", '%s.%s' % (self.__class__.__name__, current_function()))

        root = doc.documentElement

        if not root.hasAttribute(IDMESSAGE_VERSIONNODE) or root.getAttribute(IDMESSAGE_VERSIONNODE) != IDMESSAGE_VERSION:
            raise TCException("iD version mismatch", '%s.%s' % (self.__class__.__name__, current_function()))

        frame_number = int(root.getAttribute(IDMESSAGE_FRAMENODE))

        absolute = root.getAttribute(IDMESSAGE_TIMESTAMPNODE)
        self.__message.absolute.Set(absolute)
        reference = root.getAttribute(IDMESSAGE_REFERENCENODE)
        self.__message.reference.Set(reference)

        desc = root.getAttribute(IDMESSAGE_DESCRIPTIONNODE)
        self.__message.SetDescription(desc)

        ft = root.getAttribute(IDMESSAGE_FAMILYNODE)
        if ft == IDTYPES_FAMILY_BIOSIG:
            self.__message.SetFamilyType(IDMessage.FamilyBiosig)
        else:
            self.__message.SetFamilyType(IDMessage.FamilyUndef)

        ev = root.getAttribute(IDMESSAGE_EVENTNODE)
        self.__message.SetEvent(ev)

        return
