
    <?php
        $d = $_POST['score'];

        if($d<4)    echo "F";
        elseif ($d<4.5)     echo "D";
        elseif ($d<5)       echo "D+";
        elseif ($d<6)       echo "C";
        elseif ($d<7)       echo "C+";
        elseif ($d<8)       echo "B";
        elseif ($d<8.5)     echo "B+";
        else    echo "A";
    ?>