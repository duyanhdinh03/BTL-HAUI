<?php
    $x = $_POST['x'];
    $e = $_POST['e'];

    function GT($n){
        if($n == 0)
            return 1;
        else {
            return $n*GT($n-1);
        }
    }

    $i=0;
    $exp = 0;
    while(pow(x, i)/GT($i) > $e){
        $exp += pow(x, i)/GT($i);
        $i++;
    }

    echo $exp;

?>