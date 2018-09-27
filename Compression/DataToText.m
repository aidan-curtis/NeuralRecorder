function DataToText(data, output_path)
    num_channels = 1;
    for i = [1:num_channels]
        channel_x = [[1:1:length(data(i, :))]',data(i, :)'];
        csvwrite([output_path,'/A',num2str(i),'.txt'], channel_x);
    end
end

