#-----------------------------------------------------------------------


include (libTiD_settings.pri)

CONFIG  += dll shared

HARDWARE_PLATFORM = $$system(uname -m)
contains( HARDWARE_PLATFORM, x86_64 )::{
    OBJECTS_DIR = tmp/serverlib_shared/amd64
  }else::{
    OBJECTS_DIR = tmp/serverlib_shared/x68
  }

include (libTiD_src.pri)

#-----------------------------------------------------------------------
#! end of file
