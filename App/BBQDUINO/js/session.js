
$(document).ready(function(){

    //-----------------------
    // Append data to the dom
    //-----------------------
    $.ajax({
      url: 'bbq.php',
      method:"GET",
      dataType: 'json',
      success: function(data){
          var listbbq = data;
          $.ajax({
            url: 'bbqduino.php',
            method:"GET",
            dataType: 'json',
            success: function(data){
                var bbq = data.BBQID;
                var end = data.END;
                $.each(listbbq, function(index) {
                  $("#bbq").append("<option value=" + listbbq[index].ID + ">" + listbbq[index].NAME + "</option>");
                  if ((bbq  == listbbq[index].ID) && (end == 0)){
                    $("#bbq > option[value *= "+bbq+"]").attr('selected',true);
                  }
                });
          }});
    }});


    $.ajax({
      url: 'meat.php',
      method:"GET",
      dataType: 'json',
      success: function(data){
          var listmeat = data;
          $.ajax({
            url: 'bbqduino.php',
            method:"GET",
            dataType: 'json',
            success: function(data){
                var meat = data.MEATID;
                var end = data.END;
                $.each(listmeat, function(index) {
                  $("#meat").append("<option value=" + listmeat[index].ID + ">" + listmeat[index].NAME + "</option>");
                  if ((meat  == listmeat[index].ID) && (end == 0)){
                    $("#meat > option[value *= "+meat+"]").attr('selected',true);
                  }
                });
          }});
    }});


    $.ajax({
      url: 'bbqduino.php',
      method:"GET",
      dataType: 'json',
      success: function(data){
        var foodtarget = data.FOOD_TARGET;
        var bbqtarget = data.BBQ_TARGET;
        var description = data.DESCRIPTION;
        var end = data.END;
        if (end == 0){
          $("#foodtarget").val(foodtarget);
          $("#bbqtarget").val(bbqtarget);
          $("#description").val(description);
        }
      }
    });


    //---------------------
    // Modify the current session (always the last) or add a new session if the last session is already finished
    // on Submit event
    //---------------------

    $("#formsession").submit(function(event) {
      event.preventDefault();
      var form=$(this).serialize();
      console.log("form : "+form);
      $.post("managesession.php",form,function(data){
          if (data == "ok") {
            console.log("data2 :"+data);
            window.location = "bbqduino.html";
          }else{
            console.log("data3 :"+data);
            $("#status").append("<div class='alert alert-danger'>There is an error !</div>");
          }
      });
    });

});
