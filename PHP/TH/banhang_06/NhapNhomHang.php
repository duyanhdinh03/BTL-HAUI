
<?php
    $name = $_POST['name'];
    $desc = $_POST['desc'];

    include "./connect.php";
    $sql = "INSERT INTO `tblcategory`(`NAME`, `DESCR`) VALUES ('$name','$desc')";
    mysqli_query($conn, $sql);
    mysqli_close($conn);
    header("Location:NhapNhomHang.html")
?>
