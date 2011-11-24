/*
    Copyright (C) 2009-2011  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ICTYPES_HPP
#define ICTYPES_HPP
#include <string>

#define ICTYPES_ENTRY_UNDEF 		"undef"
#define ICTYPES_ENTRY_PROB 			"prob"
#define ICTYPES_ENTRY_DIST 			"dist"
#define ICTYPES_ENTRY_CLBL 			"clbl"
#define ICTYPES_ENTRY_RCOE 			"rcoe"

#define ICTYPES_LABEL_UNDEF 		"undef"
#define ICTYPES_LABEL_BIOSIG 		"biosig"
#define ICTYPES_LABEL_CLASS			"class"
#define ICTYPES_LABEL_CUSTOM 		"custom"

#define ICMESSAGE_VERSION 			"0.1.1.0"
#define ICMESSAGE_ROOTNODE 			"tobiic"
#define ICMESSAGE_VERSIONNODE		"version"
#define ICMESSAGE_CLASSNODE 		"class"
#define ICMESSAGE_CLASSISIFERNODE 	"classifier"
#define ICMESSAGE_LABELNODE			"label"
#define ICMESSAGE_NAMENODE			"name"
#define ICMESSAGE_DESCNODE			"description"
#define ICMESSAGE_LTYPENODE			"ltype"
#define ICMESSAGE_VTYPENODE			"vtype"
#define ICMESSAGE_FRAMENODE			"frame"
#define ICMESSAGE_TIMESTAMPNODE		"absolute"
#define ICMESSAGE_REFERENCENODE		"relative"

/*! \brief TOBI iC value type
 *
 * \ingroup tobiic
 *
 * ICClass value type.
 */
typedef int   		ICVtype;

/*! \brief TOBI iC value data
 *
 * \ingroup tobiic
 *
 * ICClass value data type.
 */
typedef float 		ICValue;

/*! \brief TOBI iC label type
 *
 * \ingroup tobiic
 *
 * ICClass label type.
 */
typedef int   		ICLtype;

/*! \brief TOBI iC label data
 *
 * \ingroup tobiic
 *
 * ICClass label data.
 */
typedef std::string ICLabel;

#endif

/** 
 * \defgroup tobicore TOBI Core library
 * \brief The TOBI Core library provides some basic tools used by the remaining
 * components of tobicore, such as threading stuff, time and the very useful
 * TCBlock class, from wich ICMessage and IDMessage are derived.
 *
 * \defgroup tobiic TOBI iC library
 * \brief TOBI iC implements the point-to-point encoding and decoding of
 * the classifier output data.
 *
 * \defgroup tobiid TOBI iD library
 * \brief TOBI iD implements the one-to-many encoding and decoding of
 * software events.
 *
 * \defgroup tobiplatform TOBI Platform library
 * \brief Provides implementations of the iC client/server and of an iD
 * client.
 */

/*! \mainpage tobicore
 *
 * \section sec_intro Introduction
 * The tobicore project provides a total of four C++ libraries that implement
 * some concepts of the <a href="http://www.tobi-project.org">TOBI</a> hBCI design.
 *
 * \subsection sec_intro1 libtobicore 
 * Low-level library that provides basic functionalities, such as timestamp
 * management (via TCTimestamp) and the TCBlock base class. 
 *
 * \subsection sec_intro2 libtobiic 
 * Implements the TOBI iC classifier output encoding/decoding via ICMessage.
 *
 * \subsection sec_intro3 libtobiid 
 * Implements the TOBI iD events encoding/decoding via IDMessage.
 *
 * \subsection sec_intro4 libtobiplatform
 * Provides working and cross-platform implementations of some TOBI components,
 * such as an iC client/server class (TPiC) and an iD client class (TPiD).
 * 
 * \subsection sec_intro5 tobicoremat: mextobicore, mextobiic and mextobiid
 * tobicoremat provides MEX interfaces to a large set of the tobicore
 * functionalities. The MEX interfaces are not documented here since a
 * per-funtion documentation is provided in the .m files and examples are
 * provided with the interfaces.
 *
 * \section sec_resources Resources
 * - <a href="http://www.tobi-project.org">TOBI project website</a>: general
 * resources about the big BCI project responsible about the hBCI design 
 * - <a href="http://www..org">BCI standards website</a>: lots of information
 * about hBCI and BCI standards in general
 * - <a href="http://files.mtvl.org/projects/tobicore/doc/libtobiic.pdf">TOBI iC
 * for dummies</a>: this guide was written to make sure non-technical people
 * could understand the basis of TOBI iC. Do not expect design documents and
 * other boring stuff. You will mainly find user-cases and examples there.
 * Still, it is a must-read document before starting.
 * - <a href="http://files.mtvl.org/projects/tobicore/doc/libtobiid.pdf">TOBI iD
 * for dummies</a>: as before.
 *
 * \section sec_install Compile and install
 * The tobicore libraries and tools are available on multiple platforms, and the
 * author provides ready-to-use packages. The rule of thumb is:
 * - Ubuntu and Debian Linux 32bit/64bit are always supported via .deb packages
 * - Windows support is limited to 32 bit and, as today, only for Windows 7
 * - No package is provided for other platforms
 * - GNU automake and autoconf make it easy to build the libraries on other
 * platforms
 * Some videos are available and provide an overall introduction to the
 * tobicore technicalities.
 *
 * \subsection sec_install_requirments Requirements
 * In order to build tobicore you need:
 * - automake, autoconf, libtool, gcc
 * - Mathworks Matlab for the MEX interfaces
 * - <a href="http://www.cims.nyu.edu/~dbindel/mwrap/">mwap</a>
 *
 * To compile for Microsoft Windows, you also need:
 * - MinGW + Msys
 * - <a href="http://gnumex.sourceforge.net">Gnumex</a>
 *
 * \subsection sec_install_screencast Screencast 
 * This screencast shows the procedure:
 * <a href="http://files.mtvl.org/projects/tobicore/screencasts/tobicore_build.ogv">Build tobicore on Linux and MinGW</a>
 *
 * \subsection sec_install_linux_source Tarball
 * <PRE>
 * tar xvf tobicore_3.1.0.tar.gz 
 * cd tobicore-3.1.0/
 * autoreconf -i
 * mkdir build
 * cd build
 * ../configure
 * make
 * make install # to install
 * make check   # to build the examples
 * make html    # to build the documentation
 * </PRE>
 *
 * \subsection sec_install_linux_git Git
 * <PRE>
 * git clone TODO/tobicore/
 * cd tobicore
 * git checkout -t origin/devel
 * autoreconf -i
 * mkdir build
 * cd build
 * ../configure
 * make
 * make install # to install
 * make check   # to build the examples
 * make html    # to build the documentation
 * </PRE>
 * 
 * \subsection sec_install_windows Microsoft Windows
 * Assuming your Windows installation satisfies all the requirements, you can
 * follow the Tarball or the Git steps above. 
 *
 * \subsection sec_install_linux_deb Debian packages
 * Obtain the latest .deb packages and run:
 * <PRE>
 * dpkg -i libtobicore*.deb
 * </PRE>
 *
 * \section sev_version Versioning
 * tobicore versions follow the $CURRENT.$REVISION.$AGE schema and the following
 * rules:
 * - If the library source code has changed at all since the last update,
 * then increment $REVISION
 * - If any interfaces have been added, removed, or changed since the last
 * update, increment $CURRENT, and set $REVISION to 0
 * - If any interfaces have been added since the last public release, then
 * increment $AGE
 * - If any interfaces have been removed since the last public release, then
 * set $AGE to 0
 *
 * \section sec_author Author
 * tobicore is written, documented and maintained by Michele Tavella
 * <michele.tavella@epfl.ch>.
 *
 * \section sec_ack Acknoledgment
 * This work is supported by thegno European ICT Programme Project FP7-224631,
 * TOBI: Tools for Brain-Computer Interaction. This paper only reflects the
 * authors' views and funding agencies are not liable for any use that may be made
 * of the information contained herein.
 */

/*! \example icexample.cpp
 *
 * \ingroup tobiic
 * 
 * Comprehensive TOBI iC example, using all classes.
 */

/*! \example icclass.cpp
 * A basic example on how to use ICClass.
 */

/*! \example icclassifier.cpp
 * This example shows the relation between ICClassifier and ICClass.
 */

/*! \example icserializerrapid.cpp
 * Comprehensive iC example focusing on XML serialization.
 */

/*! \example idexample.cpp
 * Simplest TOBI iD example.
 */

/*! \example idserializerrapid.cpp
 * Comprehensive iD example focusing on XML serialization.
 */

/*! \example tcblock.cpp
 * Important example that focuses on the use of blocks and timestamps.
 */

/*! \example tclanguage.cpp
 * Example showing the use of TCLanguage.
 */

/*! \example tpidclient.cpp
 * TOBI plaform iD client program.
 */

/*! \example tptcpserver.cpp
 * TOBI plaform TCP server example.
 */

/*! \example tpicclient.cpp
 * TOBI plaform iC client program.
 */

/*! \example tpicserver.cpp
 * TOBI plaform iC server program.
 */

/*! \example tptcpclient.cpp
 * TOBI plaform TCP client example.
 */
