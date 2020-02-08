//////////////////////////////////////////////////////////////////////////// 
// CLUSTER - Clustering Toolbox
//
// Copyright (c) by Dr. Eng. (J) Harald Galda, 2011
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
////////////////////////////////////////////////////////////////////////////


MacroPath = get_absolute_file_path('buildmacros.sce');

disp('Building macros  in ' + MacroPath);

tbx_build_macros("CLUSTER", MacroPath);

clear MacroPath;
