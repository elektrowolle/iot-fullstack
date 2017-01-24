<template>
  <div clas="chart-container">
    <canvas ref="chart" />
  </div>
</template>
<script>
  import Chart from "chart.js"
  import Vue   from "vue"

  function drawChart(el, {
      data   = [12, 19, 3, 5, 2, 3],
      label  = '# of Votes',
      labels = ["Red", "Blue", "Yellow", "Green", "Purple", "Orange"],
  } = {}) {
    console.log(data, label, labels);
    var myChart = new Chart(el, {
      type: 'line',
      data: {
        labels: labels,
        datasets: [{
          label: label,
          data: data,
          backgroundColor: [
            'rgba(255, 99, 132, 0.2)',
            'rgba(54, 162, 235, 0.2)',
            'rgba(255, 206, 86, 0.2)',
            'rgba(75, 192, 192, 0.2)',
            'rgba(153, 102, 255, 0.2)',
            'rgba(255, 159, 64, 0.2)'
          ],
          borderColor: [
            'rgba(255,99,132,1)',
            'rgba(54, 162, 235, 1)',
            'rgba(255, 206, 86, 1)',
            'rgba(75, 192, 192, 1)',
            'rgba(153, 102, 255, 1)',
            'rgba(255, 159, 64, 1)'
          ],
          borderWidth: 1
        }]
      },
      options: {
        scales: {
          yAxes: [{
            ticks: {
              beginAtZero: true
            }
          }]
        }
      }
    });
  }

  export default {
    name: 'chart',
    props:[
      "labels",
      "values",
      "label",
    ],
    data(){
      return {}
    },
    mounted(){
//      console.log("chart Data", this["values"]);
      drawChart(this.$refs.chart, {
        label : this.label ,
        labels: typeof this.values === typeof [] ? this.values.map((v)=>{
          let d = new Date(v.timestamp);
          return d.getHours() + ":" +
          d.getMinutes()
        }):this.labels,
        data  : typeof this.values === typeof [] ? this.values.map((v)=>Math.max(0,2500 - (v.values[0] + v.values[1] + v.values[2] + v.values[3]))) : this.values,
      });
    },
  }
</script>
<style>

</style>
