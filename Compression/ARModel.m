function[F]=ARModel(ts,h,nameAlgo)
%ts       -> time series vector
%h        -> no. step ahead
%nameAlgo -> 'direct' or 'iterated'
%F        -> vector of forecast
if strcmp(nameAlgo,'direct')==0 && strcmp(nameAlgo,'iterated')==0
    display('error: must provide correct name of algorithm: "direct" or "iterated"')
end
l=length(ts);
P=length(ts)-1;
%setting max number of lags p as following
p=round(sqrt(P)/2);
%choosing best number of lag, with AIC algorithm
AIC=zeros(p,1);
for i=1:p
    
    z=zeros(l-i,i+1);
    
    %dividing the time series into its lags
    for j=1:i+1
        z(:,j)=ts((0:end-i-1)+j);
    end
    
    y=z(:,1);    
    ly=length(y);
    
    x=z(:,2:end);
    
    phi=(x'*x)\(x'*y);
    
    yfit=x*phi;    
    e=yfit-y;
    
    sig=(e'*e)/ly;
    
    AIC(i)=log(sig)+2*i/l;
end
Aans=find(min(AIC));
%best no. of lags
p=Aans(1);
%creating dependent variable
y=ts(p+1:end);
%and its lags
x=zeros(l-p,p);
for i=1:p
    
    x(:,i)=ts(p-i+1:end-i);
    
end
%now forecasting section
F=zeros(h,1);
if strcmp(nameAlgo,'direct')
    %the forecast will be based on an AR process of y(t) onto x(t-h)
    
    for i=1:h
        
        xh=x(1:end-i+1,:);
        
        
        yh=y(i:end);
        
        size(yh)
        size(xh)
        
        phih=(xh'*xh)\(xh'*yh);
        
        F(i)=y(end-p+1:end)'*phih;
        
    end
end
if strcmp(nameAlgo,'iterated')
    %this forecast will be computed updating the time series with the last
    %forecast
    
    phi=(x'*x)\(x'*y);
    Y=y;
    for i=1:h
    
        F(i)=Y(end-p+1:end)'*phi;
    
        Y=[Y;F(i)];
    end
    F=Y(end-h+1:end);
end