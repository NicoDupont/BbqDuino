
$(document).ready(function(){

    var session=NaN;

    // compute hour and minute from a duration in millisecond
    function timeConversion(millisec) {
        var hours = parseInt((millisec / 3600000));
        //console.log(hours);
        var minutes = parseInt((millisec / 60000) - (hours*60));
        //console.log(minutes);
        if (hours < 10) {
            hours = "0"+ hours;
        }
        if (minutes < 10) {
            minutes = "0"+ minutes;
        }
        return hours + ":" + minutes;
    }

    // There is a trouble with UTC time and safari. We need to rebuild the date to get the correct local hour in safari.
    function DateSafari(timestamp) {
      var m = timestamp.substr(5, 2);
      var d = timestamp.substr(8, 2);
      var a = timestamp.substr(0, 4);
      var t = timestamp.substr(11, 8);
      if (m == 1) {
        var month = "January";
      }else if (m == 2) {
        var month = "February";
      }else if (m == 3) {
        var month = "Mars";
      }else if (m == 4) {
        var month = "April";
      }else if (m == 5) {
        var month = "May";
      }else if (m == 6) {
        var month = "Jun";
      }else if (m == 7) {
        var month = "July";
      }else if (m == 8) {
        var month = "August";
      }else if (m == 9) {
        var month = "September";
      }else if (m == 10) {
        var month = "October";
      }else if (m == 11) {
        var month = "November";
      }else if (m == 12) {
        var month = "December";
      }
      return month + " " + d + ", " + a + " " + t;
    }

    //-----------------------
    // Append data to the dom
    //-----------------------
    function LoadData(){

      $.get("bbqduino.php", function(data, status){
          if (status == "success") {

            var start = new Date(DateSafari(data.SESSION_TSTAMP));
            var end = new Date();
            var duration = timeConversion(end - start);
            $("#bbq").text(data.BBQ_NAME + " : " + data.MEAT_NAME);
            $("#time").text("Start : " + data.START_TIME.substring(0, 5) + " / Duration : " + duration);
            $("#airtemp").text("Air °C : " + data.AIR_TEMP);
            $("#bbqtemp").text(data.BBQ_TEMP);
            $("#bbqtarget").text("/" + data.BBQ_TARGET);
            $("#foodtemp").text(data.FOOD_TEMP);
            $("#foodtarget").text("/" + data.FOOD_TARGET);
            session = data.SESSION;
            var status = data.END;

            //---------------------
            // toggle the red class
            //---------------------
            var fd = 7; // food temp delta
            var bd= 7; // bbq temp delta
            var ft = $('#foodtemp');
            var bt = $('#bbqtemp');
            var foodtemp = parseInt(ft.text()); // value by id
            var bbqtemp = parseInt(bt.text());  // value by id
            var foodtarget = parseInt($('#foodtarget').text().replace("/", ""));  // value by id
            var bbqtarget = parseInt($('#bbqtarget').text().replace("/", ""));  // value by id
            if (foodtemp >= (foodtarget-fd)) {
                ft.addClass('red');
            }else{
                ft.removeClass('red');
            }
            var btinf = bbqtarget-bd;
            var btsup = bbqtarget+bd;
            if ((bbqtemp <= btinf) || (bbqtemp >= btsup)) {
                bt.addClass('red');
            }else{
                bt.removeClass('red');
            }

            //---------------------
            // Add a message if the last session is already finished
            //---------------------
            if (status == 1) {
                $("#status").append("<div class='alert alert-danger'>The session is finished ! Create a new one.</div>");
            }

        }
      },"json");

    }
    LoadData();

    //---------------------
    // line chart whit ChartJs : http://www.chartjs.org/
    //---------------------
    function LineChart() {

      //---------
      // Get data
      var datalabel = [];
      var dataair = [];
      var datafood = [];
      var databbq = [];
      var datatargetbbq = [];
      var datatargetfood = [];

      $.ajax({
        url: 'datagraph.php',
        method:"GET",
        dataType: 'json',
        async: true,
        success: function(data){
          $.each(data, function(index) {
            if (data[index].SENSOR == 1) {
              dataair.push(data[index].CELCIUS);
            }else if (data[index].SENSOR == 2) {
              databbq.push(data[index].CELCIUS);
            }else {
              datafood.push(data[index].CELCIUS);
              datalabel.push(data[index].TIME);
              datatargetbbq.push(data[index].BBQ);
              datatargetfood.push(data[index].FOOD);
            }
          });

          // make the graph after to have loaded the json
          Chart.defaults.global.legend.display = false;
          Chart.defaults.global.defaultFontColor = 'white';
          new Chart(document.getElementById("myChart"), {
            type: 'line',
            data: {
              labels: datalabel,
              datasets: [{
                  data: datafood,
                  label: "Food",
                  borderColor: "#3e95cd",
                  fill: false,
                  pointRadius: 0,
                  spanGaps: true
                }, {
                  data: databbq,
                  label: "Barbecue",
                  borderColor: "#FF5252",
                  fill: false,
                  pointRadius: 0,
                  spanGaps: true
                }, {
                  data: dataair,
                  label: "Air",
                  borderColor: "#3cba9f",
                  fill: false,
                  pointRadius: 0,
                  spanGaps: true
                }, {
                  data: datatargetbbq,
                  borderColor: "#FF5252",
                  fill: false,
                  pointRadius: 0,
                  borderDash: [5, 5],
                  borderWidth: 1,
                  spanGaps: true
                }, {
                  data: datatargetfood,
                  borderColor: "#3e95cd",
                  fill: false,
                  pointRadius: 0,
                  borderDash: [5, 5],
                  borderWidth: 1,
                  spanGaps: true
                }
              ]
            },
              options: {
                  title: {
                    display: false,
                    text: 'Temperatures'
                  },
                  legend: {
                    display: true,
                    position: 'top',
                    labels: {
                      boxWidth: 20,
                      filter: function(legendItem) {
                        // I don't want to display the label for data index 3 and 4 (start from 0) (dashed lines for the target temperatures)
                        if (legendItem.datasetIndex >= 3) {
                          return false;
                        }else {
                          return true;
                        }
                      }
                    }
                  },
                  layout: {
                      padding: {
                          left: 0,
                          right: 0,
                          top: 0,
                          bottom: 0
                      }
                  }
              }
          });
        }
      });
    }
    LineChart();

    //---------------------
    // Refresh data on click
    //---------------------

    $("#refreshdata").click(function() {
      LoadData();
      LineChart();
    });

    //---------------------
    // End the current session (always the last..) by a click on the End button at the bottom
    //---------------------
    $("#endsession").click(function() {

      $.post("endsession.php",{ID:session},function(data){
          if (data == "ok") {
            location.reload();
          }else{
            $("#status").append("<div class='alert alert-danger'>There is an error !</div>");
          }
      });

    });

});
