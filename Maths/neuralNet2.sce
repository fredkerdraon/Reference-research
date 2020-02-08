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
function classes=classes_from_outputs(outputs)
[values, classes] = max(outputs, 'c');
endfunction

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//function classes=classes_from_outputs(outputs)
//disp("Outputs");
//disp(outputs);
//[values, classes] = max(outputs, [], 2);

//[values, classes] = max(outputs, 'c');
//endfunction

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FK - Evaluation of the network 
function [regression_error, classification_error] = eval_network(data_set, weights)
[outputs, net] = feedforward(data_set.inputs, weights, data_set.bias);
//disp([outputs, net ]);
[rows, output_count] = size(weights);
//disp([rows, output_count]);
//disp(data_set.outputs);
//disp(outputs);
//disp("Soustraction");
regression_error = sum(sum((outputs - data_set.outputs) .^2)) / (data_set.count * output_count);
//disp("regression_error");
//disp(regression_error);
classes = classes_from_outputs(outputs);
//disp("Classes");
//disp(classes);
//disp("Outputs");
//disp(outputs);
//disp(classes);
//disp(data_set.classes);
data_set_classes = data_set.classes;
classification_error = sum(classes' ~= data_set_classes) / data_set.count;
//disp("class error");
//disp(classification_error);
endfunction

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FK - Feed forward of our network
function [outputs, net] = feedforward(inputs, weights, bias)
//disp("Inputs");
//disp(inputs);
//disp("Bias");
//disp(bias);
//disp(size(bias));
net = [inputs, bias] * weights;
outputs = activate(net);
//disp("Feed forward Outputs");
//disp(outputs);
//disp("Feedforward Net");
//disp(net);
endfunction

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FK - Initialise the weights
function weights=initialise_weights(max_weight, row_count, col_count)
weights = (2*rand(row_count, col_count)-1) * max_weight;
endfunction

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FK - Load datafile
function data_sets = load_data(data_file_name)
//disp(data_file_name);
//data_sets = load(data_file_name,'training','val','test','input_count','output_count','train_count','val_count','test_count');
//data_sets = load('iris3','training','val','test','input_count','output_count','train_count','val_count','test_count');
data_sets = load("iris1","training","val","test","input_count","output_count","train_count","val_count","test_count");
//load("iris3","data_sets.training","data_sets.val","data_sets.test","data_sets.input_count","data_sets.output_count","data_sets.train_count","data_sets.val_count","data_sets.test_count");
//data_sets = load('iris3',training,val,test,input_count,output_count,train_count,val_count,test_count);
//data_sets = load('iris3');
//listvarinfile("iris3");
//disp(train_count);
//disp(val);
//disp("toto");
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
//data_sets.training.bias = ones(1,data_sets.train_countx:1);
//data_sets.val.bias = ones(data_sets.val_count:1);
//data_sets.test.bias = ones(data_sets.test_count:1);
data_sets.training.count = data_sets.train_count;
data_sets.val.count = data_sets.val_count;
data_sets.test.count = data_sets.test_count;
endfunction

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FK - Generate output from classes
function outputs = outputs_from_classe(classes)
//sample_count = length(classes);
sample_count = 75;
//sample_count = size(classes(:,1));
//FK - Here we check the classes and the sample count
//disp("Classes...");
//disp(classes);
//disp("Sample count...");
//disp(sample_count);
//disp("Max classes...");
//disp(max(classes));
//disp("Length classes...");
//disp(length(classes));
outputs = zeros(sample_count, max(classes));
//disp("Zeroed outputs...");
//disp(outputs);
//disp(length(zeros));
for k = 1:sample_count
    //disp(k);
    //outputs(k, classes(k)) = 1;
    outputs = sum(classes*[1; 2; 3],'c');
    //outputs(k, max(classes)) = 1;
    //outputs(1,1) = 1;
    //disp(outputs(k,classes(k)));
    //disp("Classes index k...");
    //disp(classes(k));
end
//disp("Outputs...");
//disp(outputs);
endfunction



////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function preprocess_iris()
//printf('\tPreprocessing iris...\n');
//sample_count = 150;
sample_count = 75;
//sample_count = 60;
input_count = 4;
output_count = 3;
//train_count = round(sample_count / 2);
//test_count = round(sample_count / 4);
//val_count = sample_count - test_count - train_count;
train_count = sample_count;
test_count = ceil(sample_count / 2);
val_count = sample_count - test_count;
//disp(train_count);
//disp(test_count);
//disp(val_count);
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
    //classes = iris_data(:, 5);
    classes = iris_data(:, 5:7);
    //FK - We check the inputs and the classes
    //disp("Inputs...");
    //disp(inputs);
    //disp(classes);
    inputs = standardise_data(inputs);
    //disp("Standardized Inputs...");
    //disp(inputs);
    //outputs = outputs_from_classe(classes);i

    outputs = classes;

    //FK - Function randperm is unknown from scilab
    //class_perm = randperm(sample_count);
    //disp("We reach the random sample...");
    //class_perm = rand(sample_count);
    //class_perm = rand(sample_count,1);
    class_perm =  grand(1,'prm',(1:sample_count)');
    //disp(class_perm);
    //disp("Now we start building the training dataset ...");
    for k = 1:sample_count
        p = class_perm(k);
	//disp(k,p);
        shuffled_inputs(k,:) = inputs(p,:);
	//disp("Shuffled inputs");
	//disp(shuffled_inputs(k,:));
	//disp("Shuffled outputs");
	//disp(shuffled_inputs(k,:));
	//disp(p);
        shuffled_outputs(k,:) = outputs(p,:);
	//disp(shuffled_outputs(k,:));
        shuffled_classes(k) = classes(p);
    end
    //disp("Shuffled outputs");
    //disp(shuffled_outputs);

    //disp("Done with the suffle...");
    training.inputs = shuffled_inputs(1:train_count,:);
    //disp("Toto...");
    training.outputs = shuffled_outputs(1:train_count,:);
    //disp("Toto...");
    training.classes = shuffled_classes(1:train_count)';
    //disp("Toto...");
    val_start = 38 + 1;
    //disp(val_start);
    val_end = val_start + val_count - 1;
    //disp(val_end);
    val.inputs = shuffled_inputs(val_start:val_end,:);
    val.outputs = shuffled_outputs(val_start:val_end,:);
    val.classes = shuffled_classes(val_start:val_end)';
    test_start = 1;
    //FK - We need to define the test end............
    test_end = 38;
    //disp("Test end...");
    //disp(test_end);
    test.inputs = shuffled_inputs(test_start:test_end,:);
    test.outputs = shuffled_outputs(test_start:test_end,:);
    test.classes = shuffled_classes(test_start:test_end)';
    //disp("Done with the test inputs");
    data_set_name = sprintf('iris%d', data_set_index);
    //disp(data_set_name);
    //save(data_set_name, 'training','val','test','input_count','output_count','train_count','val_count','test_count')
    fd=mopen(data_set_name,'wb');
    //save(data_set_name, 'training','val','test','input_count','output_count','train_count','val_count','test_count')
    save(data_set_name,"training","val","test","input_count","output_count","train_count","val_count","test_count");
    //save(fd,training,val,test,input_count,output_count,train_count,val_count,test_count);
    mclose(fd);
end
endfunction

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FK - This is already implemented in scilab I guess... Check later
function y=randint(m, n, range)
y=floor(rand(m, n)*range);
endfunction

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function [weights, errors, epoch_count] = train(data_sets)
//FK - Here we provide the dataset after the preprocess_iris
//[lhs, rhs] = argn()
//disp(lhs)  // <<<<<<<<<<<
//disp("On commence le training");
//if argn(1)
    //clear;
//    disp("Pas de parametre...");
//    data_sets = load_data('iris1');
//    max_data = -1;
//    if max_data > 0
//        data_sets.training.inputs = data_sets.training.inputs(1:max_data,:);
//        data_sets.training.outputs = data_sets.training.outputs(1:max_data,:);
//        data_sets.training.classes = data_sets.training.classes(1:max_data);
//        data_sets.training.bias = data_sets.training.bias(1:max_data);
//        data_sets.training.count = max_data;
//    end
//    train(data_sets);
//    return;
//end
funcprot(0);
data_sets = load_data('iris1');
plot_data = %t;
max_weight = 1/2;
max_iterations = 1500000;
eta = .1;
val_stop_threshold = .1;
disp("Initialisation des poids");
weights = initialise_weights(max_weight, data_sets.input_count + 1, data_sets.output_count);
epoch_count = 1;
//disp(weights);
while %t 
    weights = update_backpropagation(data_sets.training.inputs,weights,data_sets.training.bias,eta,data_sets.training.outputs);    
	//disp("Training eval network");
        [train_regression_error(epoch_count),train_class_error(epoch_count)] = eval_network(data_sets.training,weights);
	//disp("Validation eval network");
        [val_regression_error(epoch_count),val_class_error(epoch_count)] = eval_network(data_sets.val,weights);
	//disp("Test eval network");
        [test_regression_error(epoch_count),test_class_error(epoch_count)] = eval_network(data_sets.test,weights);
        if modulo(epoch_count, 1000) == 0
            printf('\tEpochs: %g', epoch_count);
            printf('\tTraining: %g (%g)\n', train_regression_error(epoch_count), train_class_error(epoch_count));
            printf('\tValidation: %g (%g)\n', val_regression_error(epoch_count), val_class_error(epoch_count));
            printf('\tTest: %g (%g)\n', test_regression_error(epoch_count), test_class_error(epoch_count));
            printf('\n');            
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

printf('Training: %g (%g)\n', errors.training.regression, errors.training.classification);
printf('Validation: %g (%g)\n',errors.val.regression, errors.val.classification);
printf('Test: %g (%g)\n', errors.test.regression, errors.test.classification);

if plot_data
    //Create figure
    //figure1 = figure('PaperPosition',[0.6345 6.345 20.3 15.23],'PaperSize',[20.98 29.68]);

    //Create axes
    //plot1 = plot([train_regression_error;val_regression_error;test_regression_error;train_class_error;val_class_error;test_class_error;]');
    //plot([train_regression_error;val_regression_error;test_regression_error;train_class_error;val_class_error;test_class_error;]');
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
    //set(plot1(1),'Color',[1 0 0], 'LineStyle','-');
    //set(plot1(2),'Color',[0 .7 .7], 'LineStyle','-'); 
    //set(plot1(3),'Color',[.8 .9 0], 'LineStyle','-');
    //set(plot1(4),'Color',[1 0 0], 'LineStyle',':');
    //set(plot1(5),'Color',[0 .7 .7], 'LineStyle',':');
    //set(plot1(6),'Color',[.8 .9 0], 'LineStyle',':');

    //legend1 = legend({'Training', 'Training Classification','Validation', 'Validation Classification', 'Test', 'Test Classification'});

end
endfunction

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//function training_app_main()
//clc;
//data_sets = load_data('mushroom1');
//experiment_count = 30;
//for k=1:experiment_count
//    fprintf('-------------------------\n');
//    fprintf('Experiment %d\n', k);
//    [weights, errors, training_time(k)] = train(data_sets);
//    regression_error(k) = errors.test.regression;
//    classification_error(k) = errors.test.classification;    
//end

//fprintf('-------------------------\n');
//fprintf('Training Time\n');
//fprintf('\t %g (mean)\n', mean(training_time));
//fprintf('\t %g (std)\n', std(training_time));
//fprintf('\t %g (max)\n', max(training_time));
//fprintf('Regression Error\n');
//fprintf('\t %g (mean)\n', mean(regression_error));
//fprintf('\t %g (std)\n', std(regression_error));
//fprintf('\t %g (max)\n', max(regression_error));
//fprintf('Classification Error\n');
//fprintf('\t %g (mean)\n', mean(classification_error));
//fprintf('\t %g (std)\n', std(classification_error));
//fprintf('\t %g (max)\n', max(classification_error));
//fprintf('-------------------------\n');
//fprintf('\t %g\t%g\t%g\n', mean(training_time), std(training_time), max(training_time));
//fprintf('\t %g\t%g\t%g\n', mean(regression_error), std(regression_error), max(regression_error));
//fprintf('\t %g\t%g\t%g\n', mean(classification_error), std(classification_error), max(classification_error));
//endfunction

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
function weights=update_backpropagation(inputs, weights, bias, eta, target_outputs)
//[sample_count cols] = size(inputs);
size_inputs = size(inputs);
//disp(size_inputs);
//sample_index = randint(1, 1, sample_count) + 1;
sample_index = randint(1, 1, size_inputs(1)) + 1;
//sample_index = randint(1, 1, 75) + 1;
//disp("Sample_index");
//disp(sample_index);
//disp(inputs);
//disp("Bias");
//disp(bias);
//disp("Weights");
//disp(weights);
[output, net] = feedforward(inputs(sample_index,:), weights, bias(sample_index));
//[output, net] = feedforward(inputs(sample_index,:), weights, bias);
//disp("Input choosen");
//disp(inputs(sample_index,:));
error_test = target_outputs(sample_index,:) - output;
delta = error_test .* activate_derivative(net);
weights_delta = eta*kron([inputs(sample_index,:), bias(sample_index)]', delta);
//weights_delta = eta*kron([inputs(sample_index,:), bias]', delta);
//disp("weights_delta");
//disp(weights_delta);
weights = weights + weights_delta;
//disp("weights");
//disp(weights);
endfunction
