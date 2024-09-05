<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Danh sách</title>
    <style>
        table{
            border: 1px solid;
            border-collapse: collapse;
            width: 100%;
        }
        th, td{
            text-align: center;
            border: 1px solid;
        }
        .left{
            text-align: left;
        }
        td{
            padding: 0 10px;
            text-align: right;
        }
    </style>
</head>
<body>
    <p><a href="formnhap.html"><button>Thêm</button></a></p>
    <table>
        <tr>
            <th>STT</th>
            <th>Họ và tên</th>
            <th>Tháng</th>
            <th>Năm</th>
            <th>Số nước tiêu thụ</th>
            <th>Thành tiền</th>
            <th>Thuế VAT(10%)</th>
            <th>Thuế BVMT (15%)</th>
            <th>Tổng tiền</th>
        </tr>
        <?php 
        $count = 0;
        $fh = fopen("water.txt", "r") or die("Can't open file!");
        while(!feof($fh)){
            $count ++;
            $line = fgets($fh);
            if($line != ""){
                $temp = explode("\t", $line);
                ?>
                <tr>                    
                    <td class="left"><?=$count?></td>
                    <td class="left"><?=$temp[0]?></td>
                    <td><?=$temp[1]?></td>
                    <td><?=$temp[2]?></td>
                    <td><?=$temp[3]?></td>
                    <td><?=$temp[4]?></td>
                    <td><?=$temp[5]?></td>
                    <td><?=$temp[6]?></td>
                    <td><?=$temp[7]?></td>
                </tr>
                <?php
            }
        }
        ?>
    </table>
</body>
</html>