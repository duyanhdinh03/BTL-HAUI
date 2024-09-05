<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Bai 2</title>
    <style>
        table{
            border-collapse: collapse;
            font-size: 20px;
            width: 600px;
        }
        th, td {
            border: 1px solid black;
            padding: 8px;
            font-size: 20px;
            text-align: left;
        }
    </style>
</head>
<body>
    <table>
        <tr>
            <th>STT</th>
            <th>Họ và tên</th>
            <th>Quê quán</th>
            <th>Giới tính</th>
            <th>Năm Sinh</th>
        </tr>
        <?php         
            $arr = array(
                1 => array(1, "Nguyễn Văn Nam", "Thái Bình", "Nam", 2002),
                2 => array(2, "Nguyễn Thị Bình", "Nam Định", "Nữ", 2002),
                3 => array(3, "Nguyễn Nhật Tân", "Hải Phòng", "Nam", 2002),
                4 => array(4, "Thái Quang Hà", "Hà Nội", "Nam", 2002),
                5 => array(5, "Lê Thị Kiều", "Tuyên Quang", "Nữ", 2002),
            );

            foreach($arr as $item){
                echo "<tr>";
                echo "<td>".$item[0]."</td>";
                echo "<td>".$item[1]."</td>";
                echo "<td>".$item[2]."</td>";
                echo "<td>".$item[3]."</td>";
                echo "<td>".$item[4]."</td>";
                echo "</tr>";
            }
        ?>
    </table>
    
</body>
</html>