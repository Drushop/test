(function ($) {
    $( document ).ready(function(){
        $( ".mobile-header-search" ).click(function(){ // задаем функцию при нажатиии на элемент с классом toggle
          $( ".mobile-search-block" ).toggle(); //  скрываем, или отображаем все элементы <div>
        });
    });
    
}(jQuery));