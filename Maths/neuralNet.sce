//FK - Activation function
function y=activate(x)
y = 1*(tanh(x) + 1)/2;
endfunction

//FK - Derivative activation function
function y=activate_derivative(x)
y = 1*(1 - tanh(x).^2)/2;
endfunction

//FK - 
function classes=classes_from_outputs(outputs)
[values, classes] = max(outputs, [], 2);
endfunction

//FK - Evaluation of the network 
function [regression_error, classification_error] = eval_network(data_set, weights)
[outputs, net] = feedforward(data_set.inputs, weights, data_set.bias);
[rows, output_count] = size(weights);
regression_error = sum(sum((outputs - data_set.outputs) .^2)) / (data_set.count * output_count);
classes = classes_from_outputs(outputs);
classification_error = sum(classes ~= data_set.classes) / data_set.count;
endfunction

//FK - Feed forward of our network
function [outputs, net] = feedforward(inputs, weights, bias)
net = [inputs, bias] * weights;
outputs = activate(net);
endfunction

//FK - Initialise the weights
function weights=initialise_weights(max_weight, row_count, col_count)
weights = (2*rand(row_count, col_count)-1) * max_weight;
endfunction

//FK - Load datafile
function data_sets = load_data(data_file_name)
data_sets = load(data_file_name);
data_sets.training.bias = ones(data_sets.train_count, 1);
data_sets.val.bias = ones(data_sets.val_count, 1);
data_sets.test.bias = ones(data_sets.test_count, 1);
data_sets.training.count = data_sets.train_count;
data_sets.val.count = data_sets.val_count;
data_sets.test.count = data_sets.test_count;
endfunction

//FK - Generate output from classes
function outputs = outputs_from_classe(classes)
sample_count = length(classes);
//FK - Here we check the classes and the sample count
disp("Classes...");
//disp(classes);
disp("Sample count...");
//disp(sample_count);
disp("Max classes...");
//disp(max(classes));
outputs = zeros(sample_count, max(classes));
//disp("Zeroed outputs...");
//disp(outputs);
//disp(length(zeros));
for k = 1:sample_count
    //outputs(k, classes(k)) = 1;
    outputs(k, max(classes)) = 1;
    //outputs(1,1) = 1;
    //disp(outputs(k,classes(k)));
    //disp("Classes index k...");
    //disp(classes(k));
end
disp("Outputs...");
disp(outputs);
endfunction

function preprocess_iris()
printf('\tPreprocessing iris...\n');
sample_count = 150;
input_count = 4;
output_count = 3;
train_count = round(sample_count / 2);
test_count = round(sample_count / 4);
val_count = sample_count - test_count - train_count;
for data_set_index = 1:3
    //FK - Here we change the name of the file to match the one provided
    //fid = mopen('iris.data','r');
    //fid = mopen('iris_training.dat','r');
    //iris_data = fscanf(fid,'%g,%g,%g,%g,%g', [5, sample_count]);
    iris_data = fscanfMat('iris_training.dat');
    //status = fclose(fid);
    //FK - Here we check that we could read the matrix properly
    //disp(iris_data);
    //iris_data = iris_data'; //%transpose for visual convenience
    //disp(iris_data);
    inputs = iris_data(:, 1:4);
    //FK - Here we have a problem..........
    classes = iris_data(:, 5);
    //classes = iris_data(:, 5:7);
    //FK - We check the inputs and the classes
    disp("Inputs...");
    disp(inputs);
    //disp(classes);
    inputs = standardise_data(inputs);
    outputs = outputs_from_classe(classes);
    //FK - Function randperm is unknown from scilab
    //class_perm = randperm(sample_count);
    disp("We reach the random sample...");
    //class_perm = rand(sample_count);
    class_perm = rand(sample_count,1);
    disp(class_perm);
    for k = 1:sample_count
        p = class_perm(k);
        shuffled_inputs(k,:) = inputs(p,:);
        shuffled_outputs(k,:) = outputs(p,:);
        shuffled_classes(k) = classes(p);
    end
    training.inputs = shuffled_inputs(1:train_count,:);
    training.outputs = shuffled_outputs(1:train_count,:);
    training.classes = shuffled_classes(1:train_count)';
    val_start = train_count + 1;
    val_end = train_count + val_count;
    val.inputs = shuffled_inputs(val_start:val_end,:);
    val.outputs = shuffled_outputs(val_start:val_end,:);
    val.classes = shuffled_classes(val_start:val_end)';
    test_start = val_end + 1;
    test.inputs = shuffled_inputs(test_start:end,:);
    test.outputs = shuffled_outputs(test_start:end,:);
    test.classes = shuffled_classes(test_start:end)';
    data_set_name = sprintf('iris%d', data_set_index);
    save(data_set_name, 'training','val','test','input_count','output_count','train_count','val_count','test_count');
end
endfunction

//FK - This is already implemented in scilab I guess... Check later
function y=randint(m, n, range)
y=floor(rand(m, n)*range);
endfunction

//FK - 
function new_inputs = standardise_data(inputs)
[sample_count, cols] = size(inputs);
mean_data = mean(inputs);
new_inputs = zeros(sample_count, cols);
for k=1:sample_count
    new_inputs(k, :) = inputs(k, :) - mean_data;
end
min_data = min(new_inputs);
max_data = max(new_inputs);
divisors = max(abs(min_data), abs(max_data));
for k=1:sample_count
    new_inputs(k, :) = new_inputs(k, :) ./ divisors * .475 + 0.5;
end
endfunction

function [weights, errors, epoch_count] = train(data_sets)
if nargin == 0
    clc;
    data_sets = load_data('iris1');
    max_data = -1;
    if max_data > 0
        data_sets.training.inputs = data_sets.training.inputs(1:max_data,:);
        data_sets.training.outputs = data_sets.training.outputs(1:max_data,:);
        data_sets.training.classes = data_sets.training.classes(1:max_data);
        data_sets.training.bias = data_sets.training.bias(1:max_data);
        data_sets.training.count = max_data;
    end
    train(data_sets);
    return;
end
plot_data = true;
max_weight = 1/2;
max_iterations = 500;
eta = .1;
val_stop_threshold = .1;
weights = initialise_weights(max_weight, data_sets.input_count + 1, data_sets.output_count);
epoch_count = 1;
while true
    weights = update_backpropagation(data_sets.training.inputs,weights,data_sets.training.bias,eta,data_sets.training.outputs);    
        [train_regression_error(epoch_count),train_class_error(epoch_count)] = eval_network(data_sets.training,weights);
        [val_regression_error(epoch_count),val_class_error(epoch_count)] = eval_network(data_sets.val,weights);
        [test_regression_error(epoch_count),test_class_error(epoch_count)] = eval_network(data_sets.test,weights);
        if mod(epoch_count, 10) == 0
            fprintf('\tEpochs: %g', epoch_count);
            fprintf('\tTraining: %g (%g)\n', train_regression_error(epoch_count), train_class_error(epoch_count));
            fprintf('\tValidation: %g (%g)\n', val_regression_error(epoch_count), val_class_error(epoch_count));
            fprintf('\tTest: %g (%g)\n', test_regression_error(epoch_count), test_class_error(epoch_count));
            fprintf('\n');            
        end
    //loop maintenance    
    if (val_regression_error(epoch_count)) < val_stop_threshold | (epoch_count >= max_iterations)
        break;
    end
    epoch_count = epoch_count + 1;  
end

[errors.training.regression,errors.training.classification] = eval_network(data_sets.training, weights);
[errors.val.regression,errors.val.classification] = eval_network(data_sets.val,weights);
[errors.test.regression,errors.test.classification] = eval_network(data_sets.test,weights);

fprintf('Training: %g (%g)\n', errors.training.regression, errors.training.classification);
fprintf('Validation: %g (%g)\n',errors.val.regression, errors.val.classification);
fprintf('Test: %g (%g)\n', errors.test.regression, errors.test.classification);

if plot_data
    //Create figure
    figure1 = figure('PaperPosition',[0.6345 6.345 20.3 15.23],'PaperSize',[20.98 29.68]);

    //Create axes
    plot1 = plot([train_regression_error;val_regression_error;test_regression_error;train_class_error;val_class_error;test_class_error;]');

    title('Error vs. Learning Time');
    xlabel('Epochs');
    ylabel('Average Error');
    set(plot1(1),'Color',[1 0 0], 'LineStyle','-');
    set(plot1(2),'Color',[0 .7 .7], 'LineStyle','-'); 
    set(plot1(3),'Color',[.8 .9 0], 'LineStyle','-');
    set(plot1(4),'Color',[1 0 0], 'LineStyle',':');
    set(plot1(5),'Color',[0 .7 .7], 'LineStyle',':');
    set(plot1(6),'Color',[.8 .9 0], 'LineStyle',':');

    legend1 = legend({'Training', 'Training Classification','Validation', 'Validation Classification', 'Test', 'Test Classification'});

end
endfunction

function training_app_main()
clc;
data_sets = load_data('mushroom1');
experiment_count = 30;
for k=1:experiment_count
    fprintf('-------------------------\n');
    fprintf('Experiment %d\n', k);
    [weights, errors, training_time(k)] = train(data_sets);
    regression_error(k) = errors.test.regression;
    classification_error(k) = errors.test.classification;    
end

fprintf('-------------------------\n');
fprintf('Training Time\n');
fprintf('\t %g (mean)\n', mean(training_time));
fprintf('\t %g (std)\n', std(training_time));
fprintf('\t %g (max)\n', max(training_time));
fprintf('Regression Error\n');
fprintf('\t %g (mean)\n', mean(regression_error));
fprintf('\t %g (std)\n', std(regression_error));
fprintf('\t %g (max)\n', max(regression_error));
fprintf('Classification Error\n');
fprintf('\t %g (mean)\n', mean(classification_error));
fprintf('\t %g (std)\n', std(classification_error));
fprintf('\t %g (max)\n', max(classification_error));
fprintf('-------------------------\n');
fprintf('\t %g\t%g\t%g\n', mean(training_time), std(training_time), max(training_time));
fprintf('\t %g\t%g\t%g\n', mean(regression_error), std(regression_error), max(regression_error));
fprintf('\t %g\t%g\t%g\n', mean(classification_error), std(classification_error), max(classification_error));
endfunction

function weights=update_backpropagation(inputs, weights, bias, eta, target_outputs)
[sample_count cols] = size(inputs);
sample_index = randint(1, 1, sample_count) + 1;
[output, net] = feedforward(inputs(sample_index,:), weights, bias(sample_index));
error = target_outputs(sample_index,:) - output;
delta = error .* activate_derivative(net);
weights_delta = eta*kron([inputs(sample_index,:), bias(sample_index)]', delta);
weights = weights + weights_delta;
endfunction
