# Log Analyzer

## Features

- Parses PHP log files and extracts relevant information
- Generates an HTML page with CSS styling for an enhanced user interface
- Utilizes Chart.js to create interactive charts and graphs based on log data
- Presents statistical information and insights about the logs
- Provides customizable options for filtering and analyzing log data

## Prerequisites

- C compiler (e.g., GCC)
- PHP log file in a specified format

## Usage

1. Run the log analyzer program:

./log-analyzer <path_to_php_log_file>

2. The program will process the log file and generate an HTML page named `logs.html`.

3. Open `logs.html` in a web browser to view the log analysis results.

## Customization

- Customize the log file parsing and analysis by modifying the `log_analyzer.c` file to suit your specific requirements.
- Adjust the CSS styling in the generated HTML file (`logs.html`) to match your desired visual presentation.
- Explore the options provided by Chart.js to enhance the charts and graphs according to your preferences.

## Examples

Here are a few examples of the statistics and insights provided by the Log Analyzer:

- Number of log entries per day/week/month
- Top accessed URLs or resources
- Distribution of HTTP response codes
- Frequency of user agents or browsers
- Top IPs used
- Error rate trends over time

Feel free to explore the generated HTML page (`logs.html`) to view these examples and more.

## License

This project is licensed under the [MIT License](LICENSE). You are free to use, modify, and distribute this software.
