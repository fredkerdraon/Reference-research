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


MacroPath = get_absolute_file_path('cleanmacros.sce');

BinFiles = ls(MacroPath + '/*.bin');

for i = 1 : size(BinFiles,'*')
  
 mdelete(BinFiles(i));
  
end

mdelete(MacroPath + '\names');

mdelete(MacroPath + '\lib');

clear MacroPath;

clear BinFiles;
