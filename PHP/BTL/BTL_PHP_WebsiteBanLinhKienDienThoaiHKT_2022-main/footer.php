<!-- chatbot -->
<?php
include 'chatbot.php';
?>

<footer id="footer">
    <div class="social">
        <a href="#"><i class="fa fa-facebook" aria-hidden="true"></i></a>
        <a href="#"><i class="fa fa-twitter" aria-hidden="true"></i></a>
        <a href="#"><i class="fa fa-instagram" aria-hidden="true"></i></a>
    </div>
    <ul class="list">
        <li>
            <a href="./">Trang Chủ</a>
        </li>
        <li>
            <a href="productList.php">Sản Phẩm</a>
        </li>
    </ul>
    <p class="copyright">copy by HKT-SHOP.com 2022</p>
</footer>

<script>
    $(document).ready(function() {
        $('a').click(function(e) {
            e.preventDefault();
            var url = $(this).attr('href');
            $('#content').load(url + ' #content');
            $('#footer').load('footer.php');
        });
    });
</script>