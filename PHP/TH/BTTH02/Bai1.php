<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Bai 1</title>
    <style>
        table{
            border-collapse: collapse;
            width: 300px;
            height: 300px;
        }
        td {
            border: 1px solid black;
            padding: 8px;
            font-size: 20px;
            text-align: center;
        }
    </style>
</head>
<body>
    <table>
        <?php
            
            for($i=1; $i<=25; $i++){

            }
            $c = 1;
            for($row=1; $row<=5; $row++){
                echo "<tr>";
                for($col=1; $col<=5; $col++){
                    echo  "<td>$c</td>";
                }
                $c++;
                echo "</tr>";
            }

        ?>
    </table>
</body>
</html>