#-----------------------------------------------------------------------


include (libTiD_settings.pri)

CONFIG  += dll shared

unix{
  HARDWARE_PLATFORM = $$system(uname -m)
  contains( HARDWARE_PLATFORM, x86_64 )::{
      OBJECTS_DIR = tmp/serverlib_shared/amd64
    }else::{
      OBJECTS_DIR = tmp/serverlib_shared/x86
    }
}
include (libTiD_src.pri)

#-----------------------------------------------------------------------
#! end of file
