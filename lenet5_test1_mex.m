
%%
% 读入图片
img_test = imread('./test_images/padpic_3_57585.png');

% 数据类型转换
% img_test_double = im2double(img_test);  %TensorSet方法输入float类型数据
subplot(1,2,1)
imshow(img_test);
img_test = single(img_test);  %将uint8转为float

% 数据通道拆分
% 只需要单通道
% itd_r = img_test_double(:, :, 1);
itd_r = img_test;
% itd_g = img_test_double(:, :, 2);
% itd_b = img_test_double(:, :, 3);

% 数据拼接
itd_r1 = reshape(itd_r', [1, size(itd_r, 1) * size(itd_r, 2)]); % 数据展开成一行
% itd_g1 = reshape(itd_g', [1, size(itd_g, 1) * size(itd_g, 2)]);
% itd_b1 = reshape(itd_b', [1, size(itd_b, 1) * size(itd_b, 2)]);
img_double_array = [itd_r(:)]; % 这里只有一列
% 图片通道变换，由[tsnum, row, col, chnum] -> [tsnum, chnum, row, col]
% data_shape = [1, size(img_test_double, 3), size(img_test_double, 1), size(img_test_double, 2)];
data_shape = [1, 1, 32, 32];
ActivationFunction(itd_r1, int32(data_shape))  % ActivationFunction是mex命令生成的mexw64文件名

%%
data_from_c = importdata('test.txt');
% 数据重组
[data_from_c_tensor] = array2tensor(data_from_c, data_shape(1), data_shape(2), data_shape(3), data_shape(4));
data1(:,:,1) = data_from_c_tensor(:, 1, :, :);
% data1(:,:,2) = data_from_c_tensor(:, 2, :, :);
% data1(:,:,3) = data_from_c_tensor(:, 3, :, :);
subplot(1,2,2)
imshow(data1);