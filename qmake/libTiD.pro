#-----------------------------------------------------------------------

TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += libTiD_static.pro \
           libTiD_shared.pro \
           libTiD_client.pro \
           libTiD_lpt_client.pro \
           libTiD_sdl_client.pro \
           libTiD_client_blocking.pro \
           libTiD_server.pro \
           libTiD_tests.pro \
#           ../python/pyTiD.pro \
           ../matlab/matlabTiD.pro \
           ../matlab/runMatlabBuild.pro


libTiD_server.pro.depends = libTiD_shared.pro
libTiD_client.pro.depends = libTiD_shared.pro

libTiD_tests.pro.depends = libTiD_static.pro






#-----------------------------------------------------------------------
#! end of file
