<?php 
    $a1 = $_POST['a1'];
    $b1 = $_POST['b1'];
    $c1 = $_POST['c1'];
    
    $a2 = $_POST['a2'];
    $b2 = $_POST['b2'];
    $c2 = $_POST['c2'];
    
    $d = $a1*$b2 - $a2*$b1;
    $d1 = $c1*$b2 - $c2*$b1;
    $d2 = $a1*$c2 - $a2*$c1;

    if($d != 0){
        echo "He huong trinh co mot nghiem: <br>";
        $x = $d1/$d;
        $y = $d2/$d;
        echo "x = $x <br>";
        echo "y = $y <br>";
    }
    else{
        if($d1 = $d2 = 0){
            echo "He phuong tirnh co vo so nghiem!";
        }
        else{
            echo "He phuong tirnh vo nghiem!";
        }
    }
    
?>