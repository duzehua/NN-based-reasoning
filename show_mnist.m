%clear all  
%close all  
train_x_file='.\data\MNIST\raw\train-images-idx3-ubyte';%60000个训练集图片
test_x_file='.\data\MNIST\raw\t10k-images-idx3-ubyte'; %10000个测试集图片
train_y_file='.\data\MNIST\raw\train-labels-idx1-ubyte';%60000个训练集图片对应的数字 
test_y_file='.\data\MNIST\raw\t10k-labels-idx1-ubyte'; %10000个测试集图片对应的数字 
  
train_x=decodefile(train_x_file,'image');  
test_x=decodefile(test_x_file,'image');  
  
train_y=decodefile(train_y_file,'label');  
test_y=decodefile(test_y_file,'label');  
  
% 如果想检验转化是否正确，可执行以下代码。  
train_x_matrix=reshape(train_x,28,28,60000);%reshape后的图像是放倒的，行列颠倒   
train_x_matrix=permute(train_x_matrix,[2 1 3]);%对每张图像进行行列的转置处理，行列颠倒   
  
test_x_matrix=reshape(test_x,28,28,10000);%reshape后的图像是放倒的  
test_x_matrix=permute(test_x_matrix,[2 1 3]);%对每张图像进行行列的转置处理  
  
figure; 
axes('Position',[.1 .1 .8 .6]);
while 1
        m = unidrnd(60000);
        %imshow(train_x_matrix(:,:,m),[]);

        subplot(1,2,1);
        imshow(train_x_matrix(:,:,m));
        
        subplot(1,2,2);
        % resize后有噪点，舍弃
        %pic2 = imresize(train_x_matrix(:,:,m), [32,32]);
        %padding
        pic2 = padarray(train_x_matrix(:,:,m),[2,2]);
        imshow(pic2);
        
        rfile_name = sprintf('test_images/padpic_%i_%i.png', train_y(m), m);
        file_name = sprintf('test_images/pic_%i_%i.png', train_y(m), m);
        
        imwrite(train_x_matrix(:,:,m), file_name);
        imwrite(pic2, rfile_name);

        title(num2str(train_y(m))); 
        waitforbuttonpress;   %每点击一次随机显示一张训练集图片，与其对应的标准数字
end
