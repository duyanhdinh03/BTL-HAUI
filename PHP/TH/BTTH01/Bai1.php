<?php 
    $a = $_POST['a'];
    $b = $_POST['b'];
    $c = $_POST['c'];
    
    if($a == 0){
        echo "Đây là phương trình bậc nhất!";
        if($b==0){
            if($c == 0){
                echo "Phương trình có vô số nghiệm!";
            }
            else{
                echo "Phương trình vô nghiệm!";
            }
        }
        else{
            echo "Phuong trinh co ngiem duy nhat: -$c/$b";
        }
    }

    $delta = $b*$b - 4*$a*$c;
    if($delta < 0){
        echo "Phương tình vô nghiệm!";
    }
    else if($delta == 0){
        $x = -$b/(2*$a);
        echo "Phương tình có nghiệm kép: $x";
    }
    else{
        echo "Phương tình có 2 nghiệm phân biệt: <br>";
        $x1 = (-$b- sqrt($delta)) / (2*$a);
        $x2 = (-$b +  sqrt($delta)) / (2*$a);
        echo "x1: $x1 <br>";
        echo "x2 = $x2 <br>";
    }
?>