# Coca-Yangera-Kasami-Parser

## Чтобы поставить локально  
git clone git@github.com:Drago160/Coca-Yangera-Kasami-Parser.git  
cd Coca-Yangera-Kasami-Parser  
git checkout develop  

## Чтобы собрать проект:  
Запустите скрипт build_script  
он сам подгрузит библиотеку для тестов и соберет проект  

### Чтобы пересобрать проект(может вам понадобиться)  
cd build  
cmake ..  
make  
cd ..  

### Запуск  
main можно исполнить, исполнив build/run   
- ./build/run    
 
Тесты прогнать можно исполнив Tests/bin/BasicTests  
- ./Tests/bin/BasicTests

### Формат ввода грамматики

CF_Grammar G("S->w1|w2|w3|...|wn;A->u1|u2|u3|...|un;B->.......");  
Если нужн eps, то вместо wi, оставьте путое мето, например S->; или S->...w|; или S->|w...;  
не забывайте ставить ";"  

### Структура
- CG_Grammar - грамматика, к которой кроме построения есть метод ToHomski()  
- ToHomski приводит ее к НФ Хомского  
- CYK_Manager - в конструкторе принимает CF_Grammar, при этом требует, чтобы CF_Grammar была в НФ Хомского, иначе UB  
- У CYK_Manager'а есть метод Recognize(word) - возвращает true/false - принадлежит ли слово языку привееденной грамматики  