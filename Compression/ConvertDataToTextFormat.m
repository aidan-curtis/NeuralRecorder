num_channels = 2;
for i = [1:num_channels]
    channel_x = [[1:1:length(data(i, :))]',data(i, :)'];
    csvwrite(['TextData/A',num2str(i),'.txt'], channel_x);
end