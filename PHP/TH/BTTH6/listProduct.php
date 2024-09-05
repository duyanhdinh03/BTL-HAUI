<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>List products</title>
</head>
<body>
    <button><a href="listcart.php">Giỏ hàng</a></button>
    <table width="100%">
        <tr>
            <td>Mã hàng</td>
            <td>Tên hàng</td>
            <td>Số lượng</td>
            <td>Đơn giá</td>
            <td>Hình ảnh</td>
            <td>Chọn mua</td>
        </tr>
        <?php
        include("connection.php");

        $sqli = "SELECT * FROM listproducts";
        $result = mysqli_query($conn, $sqli);
        while($row = mysqli_fetch_assoc($result)){
            ?>
            <tr>
                <td><?=$row['id']?></td>
                <td><?=$row['name']?></td>
                <td><?=$row['quantity']?></td>
                <td><?=$row['price']?></td>
                <td>
                    <img src="<?=$row['imagepath']?>" width="180px"/>
                </td>
                <td>
                    <button><a href="cart.php?id=<?=$row['id']?>&name=<?=$row['name']?>&price=<?=$row['price']?>">Chọn mua</a></button>
                    <button><a href="edit.php?id=<?=$row['id']?>">Sửa</a></button>
                    <button type="button" onclick="Xacnhanxoa(<?=$row['id']?>)">Xóa</button>
                </td>
            </tr>
            <?php
        }
        
        mysqli_close($conn);
        ?>
    </table>
    <form action="deleteP.php" method="post" id="formXoa">
        <input type="hidden" name="id" id="id"/>
    </form>
    <script type="text/javascript">
        function Xacnhanxoa(id){
            var tl = "Bạn có thật sự muốn xóa không?";
            if(tl){
                document.getElementById("id").value = id;
                document.getElementById("formXoa").submit();
            }
            alert(id);
        }
    </script>
</body>
</html>