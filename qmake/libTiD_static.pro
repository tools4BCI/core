#-----------------------------------------------------------------------

include (libTiD_settings.pri)

CONFIG  += staticlib

HARDWARE_PLATFORM = $$system(uname -m)
contains( HARDWARE_PLATFORM, x86_64 )::{
    OBJECTS_DIR = tmp/serverlib_static/amd64
  }else::{
    OBJECTS_DIR = tmp/serverlib_static/x86
  }

include (libTiD_src.pri)

#-----------------------------------------------------------------------
#! end of file

