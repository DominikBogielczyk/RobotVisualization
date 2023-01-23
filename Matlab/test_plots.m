clear
clc

X = categorical({'0 ms','50 ms','random 50 ms','150 ms','random 150 ms','250 ms','random 250 ms','350 ms','random 350 ms','450 ms','random 450 ms'});
X = reordercats(X,{'0 ms','50 ms','random 50 ms','150 ms','random 150 ms','250 ms','random 250 ms','350 ms','random 350 ms','450 ms','random 450 ms'});
y = [17.79 18.14; 19.16 18.56; 19.39, 18.76; 22.07 19.48; 20.91 18.80; 22.94 22.80; 23.53 22.19;27.11 25.34; 26.74 23.41; 28.82 27.45; 28.65 27.81];

figure()
bar(X,y)
set(gca,'FontSize', 12)
legend(["Dominik","Micha³"],'FontSize', 14)
xlabel("Dodatkowe opóŸnienie", 'FontSize', 14)
ylabel("t [s]", 'FontSize', 14)
title("Porównanie czasów przejazdu toru",'FontSize', 18)
grid on

y2 = [0 10;0 20;0 10;10 10;0 10;10 30;30 20;40 40;30 40;40 40;50 70];
figure()
bar(X,y2)
set(gca,'FontSize', 12)
legend(["Dominik","Micha³"],'FontSize', 14)
xlabel("Dodatkowe opóŸnienie", 'FontSize', 14)
ylabel("Czêstoœæ kolizji [%]", 'FontSize', 14)
title("Porównanie czêstoœci kolizji", 'FontSize', 18)
grid on