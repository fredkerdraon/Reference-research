///////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////

global CLUSTER_PATH;

demopath = CLUSTER_PATH + 'demos\';

select getlanguage()
  
 case 'de_DE'
   
  SubDemoTitles = ['C-Means'; 'Fuzzy C-Means'; 'Selbstorganisierende Merkmalskarte (Kohonen-Map)'];
  
  case 'en_US'               
    
   SubDemoTitles = ['c-means'; 'fuzzy c-means'; 'self organizing map']; 
     
  case 'ja_JP'
    
   SubDemoTitles = ['c-means'; 'ファジー c-means'; '自己組織化マップ']; 
   
 else
   
  SubDemoTitles = ['c-means'; 'fuzzy c-means'; 'self organizing map'];
       
end;

SubDemoFiles = [demopath + 'CMeans.sce'; ...
                demopath + 'FuzzyCMeans.sce'; ...
                demopath + 'SOM.sce'];

subdemolist = cat(2, SubDemoTitles, SubDemoFiles);

clear SubDemoTitles;

clear SubDemoFiles;

