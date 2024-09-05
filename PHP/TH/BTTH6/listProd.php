<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet" 
    integrity="sha384-9ndCyUaIbzAi2FUVXJi0CjmCapSmO7SnpJef0486qhLnuZ2cdeRhO02iuK6FUUVM" crossorigin="anonymous">
    <title>List Products</title>
</head>
<body>
    <tanle class="table table-bordered">
        <thead>
            <tr>
                <th>Mã sản phẩm</th>
                <th>Sản phẩm</th>
                <th>Đơn giá</th>
            </tr>
        </thead>
        <tbody>
        <?php 
            include("configuration.php");
            $sql = "SELECT * FROM tblproduct";
            $count = 0;
            $rs = mysqli_query($conn, $sql);
            while($row = mysqli_fetch_assoc($rs)){
                $count ++;
            ?>
            <tr>
                <td><?=$count?></td>
                <td><?=$row['name']?></td>
                <td><?=$row['price']?></td>
            </tr>
            <?php 
            }
        
        ?>
        </tbody>
        
    </table>
</body>
</html>