function output=decodefile(filename,type)  
fio=fopen(filename,'r');%原始文件中数据是以2进制存储的。  
a = fread(fio,'uint8');%以8进制方式读取源文件。虽然前几项是32bit的，但是图像像素数据是8bit的，所以此处用8bit处理。   
if strcmp(type,'image')  
    output=a(17:end);%提取像素数据  
else if strcmp(type,'label')  
        output=a(9:end);        
    end  
end
% https://www.zhihu.com/question/422181127/answer/1486038225
