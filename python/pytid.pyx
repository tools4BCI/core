# distutils: language = c++
# distutils: libraries = tid tobicore tobiid boost_thread SDL

from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp cimport bool
from libcpp cimport int
import cython

string_encoding = 'ascii'

ctypedef fused numeric:
    cython.int
    cython.uint

cdef extern from "tobicore/TCTimestamp.hpp":
    cdef cppclass TCTimestamp:
        TCTimestamp( )
        void Tic()


# IDMessage - C++ interface
cdef extern from "tobiid/IDMessage.hpp":
    cdef cppclass IDMessage:
        IDMessage( )
        IDMessage( string, int )
        void Copy( IDMessage* othter )
        string GetDescription( )
        void SetDescription( string& )
        string GetSource( )
        void SetSource( string& )
        string GetFamily( )
        void SetEvent( int )
        int GetEvent( )
        void SetValue( float )
        float GetValue( )
        void Dump()
        TCTimestamp absolute

# TiD - C++ interface
cdef extern from "libtid/tid_client.h" namespace "TiD":
    cdef cppclass TiDClient:
        void connect( string, unsigned int ) except +
        void disconnect( )
        void startReceiving( )
        void stopReceiving( )
        bool receiving( )
        bool connected( )
        void setBufferSize( unsigned int )
        void reserveNrOfMsgs( unsigned int )
        void sendMessage( string& )
        void sendMessage( IDMessage& )
        void asyncSendMessage( string& )
        void asyncSendMessage( IDMessage& )
        bool newMessagesAvailable( )
        void getLastMessagesContexts( vector[IDMessage]& messages  )
        void clearMessages( )
        IDMessage wait4NewTiDMessage()

# IDMessage - Python wrapper
cdef class PyIDMessage:
    cdef IDMessage *thisptr
    def __cinit__( self, family=None, event=None ):
        if( family==None and event==None ):
            self.thisptr = new IDMessage( )
        else:
            self.thisptr = new IDMessage( family, event )
    def __dealloc__( self ):
        del self.thisptr
    def Copy( self, PyIDMessage other ):
        self.thisptr.Copy( other.thisptr )
    def GetDescription( self ):
        return self.thisptr.GetDescription( )
    def SetDescription( self, description ):
        self.thisptr.SetDescription( description.encode('utf8') )
    def GetSource( self ):
        return self.thisptr.GetSource( )
    def SetSource( self, source ):
        self.thisptr.SetSource( source.encode('utf8') )
    def GetFamily( self ):
        return string(self.thisptr.GetFamily( ))
    def SetEvent( self, event ):
        self.thisptr.SetEvent( event )
    def GetEvent( self ):
        return int(self.thisptr.GetEvent( ))
    def SetValue( self, value ):
        self.thisptr.SetValue( value )
    def GetValue( self ):
        return self.thisptr.GetValue( )
    def Dump( self ):
        self.thisptr.Dump()
    def __str__(self):
        family = self.thisptr.GetFamily().decode()
        source = self.thisptr.GetSource().decode()
        event = self.thisptr.GetEvent()
        value = self.thisptr.GetValue()
        description = self.thisptr.GetDescription().decode()
        return "Family: "+family+", Source: "+source+", Event: "+str(event)+", Value: "+str(value)+", Description: "+str(description)

# TiD - Python wrapper
cdef class PyTiDClient:
    cdef TiDClient *thisptr
    def __cinit__( self ):
        self.thisptr = new TiDClient( )
    def __dealloc__( self ):
        del self.thisptr
    def connect( self, ip, port ):
        self.thisptr.connect( ip.encode('utf8'), port )
    def disconnect( self ):
        self.thisptr.disconnect( )
    def startReceiving( self ):
        self.thisptr.startReceiving( )
    def stopReceiving( self ):
        self.thisptr.stopReceiving( )
    def receiving( self ):
        return self.thisptr.receiving( )
    def connected( self ):
        return self.thisptr.connected( )
    def setBufferSize( self, size ):
        self.thisptr.setBufferSize( size )
    def reserveNrOfMsgs( self, nr ):
        self.thisptr.reserveNrOfMsgs( nr )
    
    #def sendMessageXML( self, string msg ):
        #self.thisptr.sendMessage( msg.encode('utf8') )
    
    def sendMessage( self, PyIDMessage msg ):
        msg.thisptr.absolute.Tic( )
        self.thisptr.sendMessage( msg.thisptr[0] )    
    
    #def asyncSendMessageXML( self, string msg ):
        #self.thisptr.asyncSendMessage( msg.encode('utf8') )
        
    def asyncSendMessage( self, PyIDMessage msg ):
        self.thisptr.asyncSendMessage( msg.thisptr[0] )
    def newMessagesAvailable( self ):
        return self.thisptr.newMessagesAvailable( )
    def getLastMessagesContexts( self ):
        cdef vector[IDMessage] messages
        self.thisptr.getLastMessagesContexts( messages )
        pymsg = []
        for i in range(messages.size()):
            msg = PyIDMessage( )
            msg.thisptr.Copy( &messages[i] )
            pymsg.append(msg)
        return pymsg
    def clearMessages( self ):
        self.thisptr.clearMessages( )
    def wait4NewTiDMessage( self ):
        tmp = self.thisptr.wait4NewTiDMessage( )
        msg = PyIDMessage( )
        msg.thisptr.Copy( &tmp )
        return msg
