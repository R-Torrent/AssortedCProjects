hold on;
[m,n] = size(entero10);
axis([entero10(1,1) entero10(m,1) -1 1]);
for j = 2:n 
    plot(entero10(:,1),entero10(:,j),'k')
end
hold off;