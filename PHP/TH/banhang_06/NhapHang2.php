<?php
    $name = $_POST["name"];
    $category = $_POST["category"];
    $quantity = $_POST["quantity"];
    $price = $_POST["price"];
    $path = "./image/".basename($_FILES["image"]["name"]);

    include "./connect.php";
    $sql = "INSERT INTO `tblproduct`(`NAME`, `CATEGORY_ID`, `SO_LUONG`, `DON_GIA`, `IMAGE`) VALUES ('$name','$category','$quantity','$price','$path')";

    mysqli_query($conn, $sql);
    mysqli_close($conn);
    if (!file_exists($path))
        move_uploaded_file($_FILES["image"]["tmp_name"],$path);
    
    header("Location:NhapHang.php");
?>