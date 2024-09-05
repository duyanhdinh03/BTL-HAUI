<link rel="stylesheet" href="./css/chatbot.css">
<div class="wrapper">
    <div class="title">
        <img src="./images/Facebook_Messenger_logo_2018.svg.png" alt="">
        <div class="content-chatbot">
            <p>ChatBot</p>
            <p style="font-size: 0.8rem;">Online</p>
        </div>
    </div>
    <div class="form">
        <div class="bot-inbox inbox">
            <div class="inbox-title">
                <img src="./images/Facebook_Messenger_logo_2018.svg.png" alt="">
            </div>
            <p>Hello, how can I help you?</p>
        </div>
    </div>
    <div class="typing-field">
        <div class="input-data">
            <input id="data" type="text" placeholder="Type something here.." required>
            <button id="send-btn">Send</button>
        </div>
    </div>

</div>
<script>
    $(document).ready(function() {
        $('#data').keypress(function(e) {
            if (e.which == 13) {
                $value = $("#data").val();
                $msg = '<div class="user-inbox inbox"><img src="./images/Facebook_Messenger_logo_2018.svg.png" alt=""><p>' + $value + '</p></div></div>';
                $(".form").append($msg);
                $("#data").val('');

                // start ajax code
                $.ajax({
                    url: 'chatbot-server.php',
                    type: 'POST',
                    data: 'text=' + $value,
                    success: function(result) {
                        $replay = '<div class="bot-inbox inbox"><div class="icon"></div><img src="./images/Facebook_Messenger_logo_2018.svg.png" alt=""><p>' + result + '</p></div></div>';
                        $(".form").append($replay);
                        // when chat goes down the scroll bar automatically comes to the bottom
                        $(".form").scrollTop($(".form")[0].scrollHeight);
                    }
                });
            }
        });

        $("#send-btn").on("click", function() {
            $value = $("#data").val();
            $msg = '<div class="user-inbox inbox"><img src="./images/Facebook_Messenger_logo_2018.svg.png" alt=""><p>' + $value + '</p></div></div>';
            $(".form").append($msg);
            $("#data").val('');

            // start ajax code
            $.ajax({
                url: 'chatbot-server.php',
                type: 'POST',
                data: 'text=' + $value,
                success: function(result) {
                    $replay = '<div class="bot-inbox inbox"><div class="icon"></div><img src="./images/Facebook_Messenger_logo_2018.svg.png" alt=""><p>' + result + '</p></div></div>';
                    $(".form").append($replay);
                    // when chat goes down the scroll bar automatically comes to the bottom
                    $(".form").scrollTop($(".form")[0].scrollHeight);
                }
            });
        });
    });
</script>