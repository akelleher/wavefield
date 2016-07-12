freq = 1000;
angfreq = 2*pi*freq;
spaceX = 10000; % 5mm increments for x and y
spaceY = 5000;
numEmitters = 2;
emitterCenterX = 5000;
emitterCenterY = 0; % not yet supported
offsetX = 25;
offsetY = 0;
phaseOffset = 0; % between each emitter
phaseConstant = pi/2; %shift applied to each emitter identically
speedOfSound = 340.3; % meters/s


emitters = zeros(numEmitters, 3);
space = zeros(spaceY, spaceX);
%emitter generation
firstEmitterX = emitterCenterX-(numEmitters-1)*offsetX/2;
firstEmitterPhase = -(numEmitters-1)*phaseOffset/2;

for n = 1:numEmitters
    emitters(n,2) = firstEmitterX + (n-1)*offsetX;
    emitters(n,3) = abs(firstEmitterPhase + (n-1)*phaseOffset) + phaseConstant
    
end




tic
for n = 1:numEmitters
    parfor x = 1:spaceX
        for y = 1:spaceY
           dX = x-emitters(n,2);
           dY = y-emitters(n,1);
           dist = sqrt(dX^2 + dY^2);
           space(y,x) = space(y,x) + sin(angfreq*dist/(speedOfSound*100) + emitters(n,3));
        end
    end
end
toc

pcolor(space)
shading interp