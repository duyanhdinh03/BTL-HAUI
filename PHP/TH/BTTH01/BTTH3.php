<?php 

    $x = $_POST['x'];

    function check($n){
        $sum = 0;
        for ($i=1; $i<$n; $i++){
            if($n % $i == 0)
                $sum += $i;
        }
        if( $n == $sum ){
            return true;
        }
        else {
            return false;
        }
    }
    if( check($x)){
        echo "$x is Perfect Number <br>";
    }
    else {
        echo "$x is not Perfect Number <br>";
    }

    echo "<h2>Day so hoan hao duoi 1000</h2>";
    for ($i=1; $i<1000; $i++){
        if(check($i))
            echo "\t$i";
    }
?>