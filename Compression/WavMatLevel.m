%
% 
% Copyright (c) 2004. Brani Vidakovic
%        
%  
% ver 1.0 Built 8/24/04; ver 1.2 Built 12/1/2004
% This is Copyrighted Material
% Comments? e-mail brani@isye.gatech.edu
%   


function W=WavMatLevel(N,level,varargin)

if ischar(varargin{1})
    [h,h1,~,~]=wfilters(varargin{1});
    else
    h=varargin{1};    h1=varargin{2};
end

W = eye(N);
for i = 1:level
    temp = eye(N);
    temp(1:N/2^(i-1),1:N/2^(i-1)) = WavMat(N/2^(i-1),h,h1);
    W = temp*W;
end