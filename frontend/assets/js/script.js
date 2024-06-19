const ctxSuhuKelembaban = document.getElementById('chartSuhuKelembaban').getContext('2d');

const chartSuhuKelembaban = new Chart(ctxSuhuKelembaban, {
    type: 'line',
    data: {
        labels: [],
        datasets: [
            {
                label: 'Suhu',
                borderColor: 'red',
                yAxisID: 'y-axis-suhu',
                data: [],
                borderWidth: 1,
                fill: false
            },
            {
                label: 'Kelembaban',
                borderColor: 'blue',
                yAxisID: 'y-axis-kelembaban',
                data: [],
                borderWidth: 1,
                fill: false
            }
        ]
    },
    options: {
        responsive: true,
        scales: {
            x: {
                type: 'time',
                time: {
                    unit: 'second',
                    displayFormats: {
                        second: 'h:mm:ss a'
                    }
                }
            },
            'y-axis-suhu': {
                type: 'linear',
                position: 'left',
                beginAtZero: true,
                title: {
                    display: true,
                    text: 'Suhu (°C)'
                }
            },
            'y-axis-kelembaban': {
                type: 'linear',
                position: 'right',
                beginAtZero: true,
                title: {
                    display: true,
                    text: 'Kelembaban (%)'
                },
                grid: {
                    drawOnChartArea: false // Only want the grid lines for one axis to show up
                }
            }
        }
    }
});

const ws = new WebSocket('ws://localhost:8080');

ws.onmessage = function(event) {
    const data = JSON.parse(event.data);

    if (data.nim === 'G.231.20.0136') {
        const currentTime = new Date();

        chartSuhuKelembaban.data.labels.push(currentTime);
        chartSuhuKelembaban.data.datasets[0].data.push({
            x: currentTime,
            y: data.suhu
        });
        chartSuhuKelembaban.data.datasets[1].data.push({
            x: currentTime,
            y: data.kelembaban
        });

        if (chartSuhuKelembaban.data.labels.length > 50) {
            chartSuhuKelembaban.data.labels.shift();
            chartSuhuKelembaban.data.datasets[0].data.shift();
            chartSuhuKelembaban.data.datasets[1].data.shift();
        }

        chartSuhuKelembaban.update();

        document.getElementById('suhu').innerText = data.suhu.toFixed(2);
        document.getElementById('kelembaban').innerText = data.kelembaban.toFixed(2);
    }
};

function controlLED(pin, state) {
    const message = {
        nim: 'G.231.20.0136',
        messages: `${pin}=${state}`
    };
    ws.send(JSON.stringify(message));
}

ws.onopen = function() {
    console.log('WebSocket connection established');
};

ws.onclose = function() {
    console.log('WebSocket connection closed');
};

ws.onerror = function(error) {
    console.error('WebSocket error:', error);
};
