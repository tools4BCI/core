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
 * \brief Provide some stuff to do stuff
 *
 * \defgroup tobiic TOBI iC library
 * \brief Provide some stuff to do stuff
 *
 * \defgroup tobiid TOBI iD library
 * \brief Provide some stuff to do stuff
 *
 * \defgroup tobiplatform TOBI Platform library
 * \brief Provide some stuff to do stuff
 *
 */

/*! \mainpage tobicore
 *
 * \section sec_intro Introduction
 * todo
 *
 * \section sec_install Install
 * todo
 *
 * \subsection sec_install_linux_source Tarball/Git
 * todo
 *
 * \subsection sec_install_linux_deb Debian packages
 * todo
 * 
 * \subsection sec_install_windows Microsoft Windows
 * todo
 *
 * \section sec_resources Resources
 * todo
 *
 * \section sec_author Author
 * todo
 *
 */


