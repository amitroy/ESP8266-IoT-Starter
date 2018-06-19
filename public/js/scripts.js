$(function () {
    $("#slider-fill").on("slidestop", function (event) {
        $.post(
            "https://iz7g8bf4t4.execute-api.us-east-1.amazonaws.com/exp/v1/api/led-state",
            JSON.stringify({
                enableLED: true,
                brightness: event.target.value
            }),
            function (response) {});
    });
    $("#btn-on").click(function (event) {
        $.post("https://iz7g8bf4t4.execute-api.us-east-1.amazonaws.com/exp/v1/api/led-state",
            JSON.stringify({
                enableLED: true,
                brightness: $("#slider-fill").val()
            }),
            function (response) {});
    });
    $("#btn-off").click(function (event) {
        $.post("https://iz7g8bf4t4.execute-api.us-east-1.amazonaws.com/exp/v1/api/led-state",
            JSON.stringify({
                enableLED: false
            }),
            function (response) {});
    });
    $("#set-brightness").click(function (event) {
        $.post("https://iz7g8bf4t4.execute-api.us-east-1.amazonaws.com/exp/v1/api/led-state",
            JSON.stringify({
                enableLED: true,
                brightness: $("#slider").slider("value")
            }),
            function (response) {});
    })
});