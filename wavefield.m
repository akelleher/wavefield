freq = 400;
angfreq = 2*pi*freq;
spaceX = 10000; % 5mm increments for x and y
spaceY = 5000;
numEmitters = 10;
emitterCenterX = 5000;
emitterCenterY = 0; % not yet supported
speedOfSound = 340.3; % meters/s
wavelength = speedOfSound/freq
offsetX = 25; %mm
offsetY = 0;
theta = pi/2; %radians
phaseOffset = 2*pi*(offsetX/1000)*sin(theta)/wavelength; % between each emitter


emitters = zeros(numEmitters, 3);
space = zeros(spaceY, spaceX);
%emitter generation
firstEmitterX = emitterCenterX-(numEmitters-1)*offsetX/2;
firstEmitterPhase = -(numEmitters-1)*phaseOffset/2;

for n = 1:numEmitters
    emitters(n,2) = firstEmitterX + (n-1)*offsetX;
    emitters(n,3) = (n-1)*phaseOffset;
    
end

[X,Y] = meshgrid(1:spaceX, 1:spaceY);

for n = 1:numEmitters
    tic
    distances = hypot(X-emitters(n,2), Y-emitters(n,1));
    toc
    tic
    space = space + sin((angfreq*gather(distances)/(speedOfSound*100)) + emitters(n,3));
    toc
end

pcolor(space)
shading interp