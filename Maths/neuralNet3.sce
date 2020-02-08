////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FK - Activation function
function y=activate(x)
y = 1*(tanh(x) + 1)/2;
endfunction

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FK - Derivative activation function
function y=activate_derivative(x)
y = 1*(1 - tanh(x).^2)/2;
endfunction

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FK - 
//function classes=classes_from_outputs(outputs)
//[values, classes] = max(outputs, [], 2);
//endfunction

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FK - Evaluation of the network 
function [regression_error, classification_error] = eval_network(data_set, weights)
[outputs, net] = feedforward(data_set.inputs, weights, data_set.bias);
[rows, output_count] = size(weights);
regression_error = sum(sum((outputs - data_set.outputs) .^2)) / (data_set.count * output_count);
classes = classes_from_outputs(outputs);
data_set_classes = data_set.classes;
classification_error = sum(classes' ~= data_set_classes) / data_set.count;
endfunction

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FK - Feed forward of our network
function [outputs, net] = feedforward(inputs, weights, bias)
//disp("Feedforward");
//disp(bias);
net = [inputs, bias] * weights;
//disp("Net");
//disp(net);
//disp("Input");
//disp(inputs);
//disp("Bias");
//disp(bias);
//disp("Weights");
//disp(weights);
outputs = activate(net);
//disp("Activate");
//disp(outputs);
endfunction

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FK - Initialise the weights
function weights=initialise_weights(max_weight, row_count, col_count)
weights = (2*rand(row_count, col_count)-1) * max_weight;
endfunction

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FK - Load datafile
function data_sets = load_data(data_file_name)
data_sets = load("iris1","training","val","test","input_count","output_count","train_count","val_count","test_count");
data_sets.training = training;
data_sets.val = val;
data_sets.test = test;
data_sets.input_count = input_count;
data_sets.output_count = output_count;
data_sets.train_count = train_count;
data_sets.val_count = val_count;
data_sets.test_count = test_count;
data_sets.training.bias = ones(data_sets.train_count, 1);
data_sets.val.bias = ones(data_sets.val_count, 1);
data_sets.test.bias = ones(data_sets.test_count, 1);
data_sets.training.count = data_sets.train_count;
data_sets.val.count = data_sets.val_count;
data_sets.test.count = data_sets.test_count;
endfunction

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FK - Generate output from classes
function outputs = outputs_from_classe(classes)
sample_count = 75;
outputs = zeros(sample_count, max(classes));
for k = 1:sample_count
    outputs = sum(classes*[1; 2; 3],'c');
end
endfunction

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function classes=classes_from_outputs(outputs)
[values, classes] = max(outputs, 'c');
endfunction

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function preprocess_iris()
sample_count = 75;
input_count = 4;
output_count = 3;
train_count = sample_count;
test_count = ceil(sample_count / 2);
val_count = sample_count - test_count;
for data_set_index = 1:3
    //FK - Here we change the name of the file to match the one provided
    iris_data = fscanfMat('iris_training.dat');
    inputs = iris_data(:, 1:4);
    classes = iris_data(:, 5:7);
    subplot(221);
    histplot(100,inputs(:,1), style=2);
    //x = cdfnor("X",mean(input(:,1),1,.5,.5));
    //x = cdfnor("X",0,1,.5,.5);
    //plot(x);
    //disp(x);
    title("Raw data column 1");
    subplot(222);
    histplot(100,inputs(:,1),normalization=%f);
    title("Input with graphic normailization column 1");
    inputs = standardise_data(inputs);
    subplot(223);
    histplot(100,inputs(:,1));
    title("Standardized data column 1");
    subplot(224);
    histplot(100,inputs(:,1),normalization=%f);
    title("Standardized data with graphic standardization column 1");
    outputs = classes;
    classes = outputs_from_classe(outputs);
    //disp(outputs);
    //disp(classes);
    class_perm =  grand(1,'prm',(1:sample_count)');
    //disp(class_perm);
    for k = 1:sample_count
        p = class_perm(k);
        shuffled_inputs(k,:) = inputs(p,:);
        shuffled_outputs(k,:) = outputs(p,:);
        shuffled_classes(k) = classes(p);
    end
    //disp(shuffled_inputs);
    //disp(shuffled_outputs);
    //disp(shuffled_classes);
    training.inputs = shuffled_inputs(1:train_count,:);
    training.outputs = shuffled_outputs(1:train_count,:);
    training.classes = shuffled_classes(1:train_count)';
    val_start = 38 + 1;
    val_end = val_start + val_count - 1;
    val.inputs = shuffled_inputs(val_start:val_end,:);
    val.outputs = shuffled_outputs(val_start:val_end,:);
    val.classes = shuffled_classes(val_start:val_end)';
    test_start = 1;
    //FK - We need to define the test end............
    test_end = 38;
    test.inputs = shuffled_inputs(test_start:test_end,:);
    test.outputs = shuffled_outputs(test_start:test_end,:);
    test.classes = shuffled_classes(test_start:test_end)';
    data_set_name = sprintf('iris%d', data_set_index);
    fd=mopen(data_set_name,'wb');
    save(data_set_name,"training","val","test","input_count","output_count","train_count","val_count","test_count");
    mclose(fd);
end
endfunction

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FK - This is already implemented in scilab I guess... Check later
function y=randint(m, n, range)
y=floor(rand(m, n)*range);
//disp("Randint");
//disp(m);
//disp(n);
//disp(range);
//disp(y);
endfunction

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FK - 
function new_inputs = standardise_data(inputs)
//disp("call to standardization");
[sample_count, cols] = size(inputs);
//disp(inputs);
//FK - Mean of the whole set of measures (for Iris data it's 0.5098)
mean_data = mean(inputs);
//disp(mean_data);
//FK - New inputs, all zeros with the same numbers of rows and columns
new_inputs = zeros(sample_count, cols);
//disp(new_inputs);
//FK - We just subtract the mean to all the inputs
for k=1:sample_count
    new_inputs(k, :) = inputs(k, :) - mean_data;
end
//disp(new_inputs);
//FK - Minimum value in the table
min_data = min(new_inputs);
//disp(min_data);
max_data = max(new_inputs);
//disp(max_data);
//FK - Maximum of the previous values
divisors = max(abs(min_data), abs(max_data));
//disp(divisors);
//disp(sample_count);
for k=1:sample_count
    new_inputs(k, :) = new_inputs(k, :) ./ divisors * .475 + 0.5;
end
//disp(new_inputs);
endfunction

function new_inputs = standardise_data_new(inputs)
[lambda,facpr,comprinc] = pca(inputs);
new_inputs = comprinc;
//FK - Debug
//disp(new_inputs);
endfunction

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function [weights, errors, epoch_count] = train(data_sets)
//FK - Here we provide the dataset after the preprocess_iris
funcprot(0);
data_sets = load_data('iris1');
plot_data = %t;
//max_weight = 3/4;
max_weight = 1/2;
max_iterations = 1500000;
//max_iterations = 1;
eta = .1;
val_stop_threshold = .1;
//FK - Debug
//disp("Initialisation des poids");
//disp(data_sets.input_count + 1);
//disp(data_sets.output_count);
weights = initialise_weights(max_weight, data_sets.input_count + 1, data_sets.output_count);
//disp(weights);
epoch_count = 1;
while %t 
    //disp("Boucle");
    weights = update_backpropagation(data_sets.training.inputs,weights,data_sets.training.bias,eta,data_sets.training.outputs);    
    //disp(weights);
    //weights_history[epoch_count] = weights;
	//hist3d(weights);
	//f0=scf(0);
	//surf(weights,'facecol','red','edgecol','blu');
	//surf(weights,'colorda',ones(10,10),'edgeco','cya','marker','penta','markersiz',20,'markeredg','yel','ydata',56:65);
        [train_regression_error(epoch_count),train_class_error(epoch_count)] = eval_network(data_sets.training,weights);
        [val_regression_error(epoch_count),val_class_error(epoch_count)] = eval_network(data_sets.val,weights);
        [test_regression_error(epoch_count),test_class_error(epoch_count)] = eval_network(data_sets.test,weights);
        if modulo(epoch_count, 1000) == 0
            printf('\tEpochs: %g', epoch_count);
            printf('\tTraining: %g (%g)\n', train_regression_error(epoch_count), train_class_error(epoch_count));
            printf('\tValidation: %g (%g)\n', val_regression_error(epoch_count), val_class_error(epoch_count));
            printf('\tTest: %g (%g)\n', test_regression_error(epoch_count), test_class_error(epoch_count));
            printf('\n');            
        end
	//disp(epoch_count);
        //disp(max_iterations);
    //loop maintenance    
    if (val_regression_error(epoch_count)) < val_stop_threshold | (epoch_count >= max_iterations)
	//disp("Break");
	//disp(epoch_count);
	//disp(max_iterations);
        break;
    end
    epoch_count = epoch_count + 1;  
end

[errors.training.regression,errors.training.classification] = eval_network(data_sets.training, weights);
[errors.val.regression,errors.val.classification] = eval_network(data_sets.val,weights);
[errors.test.regression,errors.test.classification] = eval_network(data_sets.test,weights);

printf('Training: %g (%g)\n', errors.training.regression, errors.training.classification);
printf('Validation: %g (%g)\n',errors.val.regression, errors.val.classification);
printf('Test: %g (%g)\n', errors.test.regression, errors.test.classification);

if plot_data
    f0=scf(1);
    subplot(211)
    plot(train_regression_error',"r");
    plot(val_regression_error',"b");
    plot(test_regression_error',"g");
    title('Regression Error vs. Learning Time');
    xlabel('Epochs');
    ylabel('Average Error');
    
    subplot(212)
    plot(train_class_error',"m");
    plot(val_class_error',"k");
    plot(test_class_error',"c");

    title('Classification Error vs. Learning Time');
    xlabel('Epochs');
    ylabel('Average Error');

end
endfunction

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function training_app_main()
//clc;
data_sets = load_data('mushroom1');
experiment_count = 30;
for k=1:experiment_count
//    fprintf('-------------------------\n');
//    fprintf('Experiment %d\n', k);
    [weights, errors, training_time(k)] = train(data_sets);
    regression_error(k) = errors.test.regression;
    classification_error(k) = errors.test.classification;    
end

printf('-------------------------\n');
printf('Training Time\n');
printf('\t %g (mean)\n', mean(training_time));
printf('\t %g (std)\n', stdev(training_time));
printf('\t %g (max)\n', max(training_time));
printf('Regression Error\n');
printf('\t %g (mean)\n', mean(regression_error));
printf('\t %g (std)\n', stdev(regression_error));
printf('\t %g (max)\n', max(regression_error));
printf('Classification Error\n');
printf('\t %g (mean)\n', mean(classification_error));
printf('\t %g (std)\n', stdev(classification_error));
printf('\t %g (max)\n', max(classification_error));
printf('-------------------------\n');
printf('\t %g\t%g\t%g\n', mean(training_time), stdev(training_time), max(training_time));
printf('\t %g\t%g\t%g\n', mean(regression_error), stdev(regression_error), max(regression_error));
printf('\t %g\t%g\t%g\n', mean(classification_error), stdev(classification_error), max(classification_error));
endfunction

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function weights=update_backpropagation(inputs, weights, bias, eta, target_outputs)
size_inputs = size(inputs);
//disp(size_inputs);
sample_index = randint(1, 1, size_inputs(1)) + 1;
//disp("Sample index");
//disp(sample_index);
[output, net] = feedforward(inputs(sample_index,:), weights, bias(sample_index));
//FK - Debug
//disp("Input");
//disp(inputs(sample_index,:));
//disp("Bias");
//disp(bias(sample_index));
//disp("Output");
//disp(output);
//disp("Weights");
//disp(weights);
//disp("Net");
//disp(net);
//disp([output,net]);
error_test = target_outputs(sample_index,:) - output;
//disp("Expected output");
//disp(target_outputs(sample_index,:));
//disp("Error test");
//disp(error_test);
//disp("Net");
//disp(net);
delta = error_test .* activate_derivative(net);
//disp("Delta");
//disp(delta);
weights_delta = eta*kron([inputs(sample_index,:), bias(sample_index)]', delta);
//disp("Inputs sample index");
//disp(inputs(sample_index,:));
//disp("Bias sample index");
//disp(bias(sample_index));
//disp("Weighs delta");
//disp(weights_delta);
weights = weights + weights_delta;
endfunction

//inputs=[1 2 3; 3 4 5; 6 7 8; 9 10 11; 12 13 14; 15 16 17; 18 19 20];
//bias=[1;1;1;1;1;1;1]
//weights=[2 2 2; 2 2 2;2 2 2;2 2 2];
//net=[inputs, bias];
//net=[inputs, bias]*weights;
//x=[-2*%pi:0.1:2*%pi]';
//plot(1*(tanh(x) + 1)/2);
//plot(1*(1 - tanh(x).^2)/2);
//function y=activate(x)
//y = 1*(tanh(x) + 1)/2;
//endfunction
//function y=activate_derivative(x)
//y = 1*(1 - tanh(x).^2)/2;
//endfunction

