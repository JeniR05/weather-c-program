#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct Memory {
    char *response;
    size_t size;
};

size_t write_callback(void *data, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    struct Memory *mem = (struct Memory *)userp;

    char *ptr = realloc(mem->response, mem->size + total_size + 1);
    if(ptr == NULL) {
        return 0;
    }

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, total_size);
    mem->size += total_size;
    mem->response[mem->size] = '\0';

    return total_size;
}

int main() {
    CURL *curl;
    CURLcode res;
    char city[50];
    char url[256];

    const char *API_KEY = "YOUR_API_KEY_HERE";   // Replace with your API key

    printf("Simple Weather Application\n");
    printf("==========================\n");
    printf("Enter city name: ");
    scanf("%s", city);

    snprintf(url, sizeof(url),
             "https://api.openweathermap.org/data/2.5/weather?q=%s&appid=%s&units=metric",
             city, API_KEY);

    struct Memory chunk;
    chunk.response = malloc(1);
    chunk.size = 0;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);

        if(res == CURLE_OK) {
            printf("\nWeather Data (JSON Output):\n");
            printf("---------------------------------\n");
            printf("%s\n", chunk.response);
        } else {
            printf("Error fetching weather data.\n");
        }

        curl_easy_cleanup(curl);
    }

    free(chunk.response);
    return 0;
}
