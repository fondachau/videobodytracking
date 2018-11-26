clear

%reading motion from file
fileID=fopen('outnet.txt','r');%read from file
formatSpec = '%f %f'; %each row will have two floats
sizeA = [2 Inf]; %to infinity
motion = fscanf(fileID,formatSpec,sizeA); %store read value on motion
fclose(fileID);
Fs=motion(1,1); %frequency rate 
frames=Fs;
%Fs=1000/Fs;
motion(1,1)=0;
motion=-motion; %original is current minus previous, so want position motion for moving forward

%converting motion to vectors
displacementy=zeros(length(motion),1);
for i=1:length(motion)
    displacementy(i+1)=displacementy(i)+motion(2,i);
end

displacementy=detrend(displacementy);
%do fourier transform, need to do large frequence so get decent resolution
L=50000;
n = 2^nextpow2(L);
%Fs=100;
f = Fs*(0:(n/2))/n;
%filter range of below 2hz
range3Hz=3/(Fs/n);
rangepoint1Hz=0.1/(Fs/n);
Y = fft(displacementy,n);
P = abs(Y/n);
%plot(f,P(1:n/2+1)) 
%plot(f(1:20000),P(1:20000))
plot(f(round(rangepoint1Hz):round(range3Hz)),P(round(rangepoint1Hz):round(range3Hz)))


%find peaks of fft
[peaks,loc] = findpeaks(P(round(rangepoint1Hz):round(range3Hz)));
loc=loc+round(rangepoint1Hz);
x=zeros(round(length(peaks)/2),2);
i=1;
while(loc(i)<0.5)
    i=i+1;
end

%find the maximum peaks 
%assume max peak is resporatory rate
maxx=max(peaks);
for i=1:round(length(peaks))
    x(i,1)=peaks(i);
    x(i,2)=loc(i);
        if(maxx==x(i,1))
            resporatory=f(loc(i))
        end
end

%convert to Beats per min
BPMin=resporatory*60
BPvideo=resporatory*400/Fs
