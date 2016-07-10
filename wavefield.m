freq = 1000;
angfreq = 2*pi*freq;
spaceX = 1000; % 5mm increments for x and y
spaceY = 500;
emitters = [0, 500; 0,510; 0, 490; 0, 520; 0, 530; 0, 480; 0, 470; 0, 460; 0, 450; 0, 440; 0, 540; 0, 550; 0, 560];
numEmitters = 13;
space = zeros(spaceY, spaceX);
tic
for n = 1:numEmitters
    for x = 1:spaceX
        for y = 1:spaceY
           dX = x-emitters(n,2);
           dY = y-emitters(n,1);
           dist = sqrt(dX^2 + dY^2);
           space(y,x) = space(y,x) + sin(angfreq*dist);
        end
    end
end
toc
pcolor(space)
shading interp