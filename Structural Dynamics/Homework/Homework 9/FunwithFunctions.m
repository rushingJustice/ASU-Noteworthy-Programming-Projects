clc; clear;
c = 10;
a = 3;
b = 1;

for i = 1:25
x(i) = i;    
f(i) = c*(i - a)^3 + b;
end
plot(x,f)