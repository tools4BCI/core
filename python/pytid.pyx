# distutils: language = c++
# distutils: libraries = tid tobicore tobiid boost_thread SDL

from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp cimport bool

# IDMessage - C++ interface
cdef extern from "tobiid/IDMessage.hpp":
    cdef cppclass IDMessage:
        IDMessage( )
        IDMessage( int, int )
        void Copy( IDMessage* othter )
        string GetDescription( )
        void SetDescription( string& )
        string GetSource( )
        void SetSource( string& )
        string GetFamily( )
        void SetFamilyType( int )
        void SetFamilyType( string& )
        int GetFamilyType( )
        void SetEvent( int )
        int GetEvent( )
        void SetValue( float )
        float GetValue( )

# TiD - C++ interface
cdef extern from "libtid/tid_client.h" namespace "TiD":
    cdef cppclass TiDClient:
        void connect( string, unsigned int ) except +
        void disconnect( )
        void startReceiving( )
        void stopReceiving( )
        bool receiving( )
        void setBufferSize( unsigned int )
        void reserveNrOfMsgs( unsigned int )
        void sendMessage( string& )
        void sendMessage( IDMessage& )
        void AsyncSendMessage( string& )
        void AsyncSendMessage( IDMessage& )
        bool newMessagesAvailable( )
        void getLastMessagesContexts( vector[IDMessage]& messages  )
        void clearMessages( )

# IDMessage - Python wrapper
cdef class PyIDMessage:
    cdef IDMessage *thisptr
    def __cinit__( self, familyType=None, event=None ):
        if( familyType==None and event==None ):
            self.thisptr = new IDMessage( )
        else:
            self.thisptr = new IDMessage( familyType, event )
    def __dealloc__( self ):
        del self.thisptr
    def Copy( self, PyIDMessage other ):
        self.thisptr.Copy( other.thisptr )
    def GetDescription( self ):
        return self.thisptr.GetDescription( )
    def SetDescription( self, description ):
        self.thisptr.SetDescription( description )
    def GetSource( self ):
        return self.thisptr.GetSource( )
    def SetSource( self, source ):
        self.thisptr.SetSource( source )
    def GetFamily( self ):
        return self.thisptr.GetFamily( )
    def SetFamilyTypeInt( self, int type ):
        self.thisptr.SetFamilyType( type )
    def SetFamilyTypeString( self, string type ):
        self.thisptr.SetFamilyType( type )
    def GetFamilyType( self ):
        return self.thisptr.GetFamilyType( )
    def SetEvent( self, event ):
        self.thisptr.SetEvent( event )
    def GetEvent( self ):
        return self.thisptr.GetEvent( )
    def SetValue( self, value ):
        self.thisptr.SetValue( value )
    def GetValue( self ):
        return self.thisptr.GetValue( )
    def __str__(self):
        family = self.thisptr.GetFamily().decode()
        source = self.thisptr.GetSource().decode()
        family_type = self.thisptr.GetFamilyType()
        event = self.thisptr.GetEvent()
        value = self.thisptr.GetValue()
        description = self.thisptr.GetDescription().decode()
        return "Family: "+family+", Source: "+source+", FType: "+str(family_type)+", Event: "+str(event)+", Value: "+str(value)+", Description: "+str(description)

# TiD - Python wrapper
cdef class PyTiDClient:
    cdef TiDClient *thisptr
    def __cinit__( self ):
        self.thisptr = new TiDClient( )
    def __dealloc__( self ):
        del self.thisptr
    def connect( self, ip, port ):
        self.thisptr.connect( ip, port )
    def disconnect( self ):
        self.thisptr.disconnect( )
    def startReceiving( self ):
        self.thisptr.startReceiving( )
    def stopReceiving( self ):
        self.thisptr.stopReceiving( )
    def receiving( self ):
        return self.thisptr.receiving( )
    def setBufferSize( self, size ):
        self.thisptr.setBufferSize( size )
    def reserveNrOfMsgs( self, nr ):
        self.thisptr.reserveNrOfMsgs( nr )
    def sendMessageXML( self, string msg ):
        self.thisptr.sendMessage( msg )
    def sendMessage( self, PyIDMessage msg ):
        self.thisptr.sendMessage( msg.thisptr[0] )
    def AsyncSendMessageXML( self, string msg ):
        self.thisptr.AsyncSendMessage( msg )
    def AsyncSendMessage( self, PyIDMessage msg ):
        self.thisptr.AsyncSendMessage( msg.thisptr[0] )
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
