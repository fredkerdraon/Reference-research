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

mode(1)

// Demo of self organizing map

global CLUSTER_PATH;

Source = read_csv(CLUSTER_PATH + 'demos\IrisData.csv', ascii(9));

Samples = strtod(Source(:, 1 : 4));

GridSize = [5 3];

InitialWeights = InitSOM(Samples, GridSize, 'equidistant');

LearningRateStruct = struct('Max', 0.2, 'Min', 0.02, 'Duration', size(Samples, 1));

function [Result] = LinearLearningRate(TimeStep, Parameter)
  Result = Parameter.Max - (Parameter.Max - Parameter.Min) * (TimeStep - 1) / (Parameter.Duration - 1);  
endfunction

NeighborhoodStruct = struct('Max', 5, 'Duration', size(Samples, 1));

function [Result] = CircularNeighborhood(Distance, TimeStep, Parameter)
  MaxDistance = Parameter.Max - (Parameter.Max - 1) * (TimeStep - 1) / (Parameter.Duration - 1);
  Result = double(Distance <= MaxDistance);
endfunction

SOMWeights = TrainSOM(Samples, ...
                      InitialWeights, ...
                      GridSize, ...
                      LinearLearningRate, ...
                      LearningRateStruct, ...
                      CircularNeighborhood, ...
                      NeighborhoodStruct, ...
                      2)
