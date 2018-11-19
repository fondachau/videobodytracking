clear
fileID=fopen('outnet.txt','r');
formatSpec = '%f %f';
sizeA = [2 Inf];
motion = fscanf(fileID,formatSpec,sizeA);
Fs=motion(1,1);
frames=Fs;
%Fs=1000/Fs;
motion(1,1)=0;
motion=-motion;
displacementy=zeros(length(motion),1);
for i=1:length(motion)
    displacementy(i+1)=displacementy(i)+motion(2,i);
end

L=50000;
n = 2^nextpow2(L);
%Fs=100;
f = Fs*(0:(n/2))/n;
Y = fft(displacementy,n);
P = abs(Y/n);
%plot(f,P(1:n/2+1)) 
plot(f(1:20000),P(1:20000))
x=zeros(length(peaks),2);
[peaks,loc] = findpeaks(P);
x=zeros(round(length(peaks)/2),2);
i=1;
while(loc(i)<0.5)
    i=i+1;
end

maxx=max(peaks(10:300))
for i=1:round(length(peaks)/2)
    x(i,1)=peaks(i);
    x(i,2)=loc(i);
        if(maxx==x(i,1))
            i
            resporatory=f(loc(i))
        end
    
    
end
BPMin=resporatory*60
BPvideo=resporatory*400/Fs