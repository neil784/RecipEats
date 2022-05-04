#include "../includes/retriever.hpp"
    
    Retriever::Retriever(std::vector<std::string>& ingredients, size_t num_results): num_of_results_(num_results) {
        ingredients_ = ingredients;
        AssembleHtml();
        if (web_link_.empty()) {
            throw std::out_of_range("invalid website");
        }
        TopResults(num_results);
    }
    void Retriever::AssembleHtml() {
        std::string assemble = "https://www.allrecipes.com/search/results/?search=";
        for (const std::string& ingredient : ingredients_) {
            assemble.append("&IngIncl=");
            assemble.append(ingredient);
        }
        web_link_ = assemble;
        
    }

    void Retriever::TopResults(size_t num) {
        CurlObj data(web_link_);
        html_data_ = data.GetData();
        std::string sub = "class=\"card__titleLink manual-link-behavior\"\n                            href=";
        std::vector<size_t> recipe_positions;
        size_t pos = html_data_.find(sub, 0);
        while(pos != std::string::npos)
        {
            recipe_positions.push_back(pos);
            pos = html_data_.find(sub,pos+1);
        }
        if (num > recipe_positions.size()) {
            num = recipe_positions.size();
        }
        for (size_t i = 0; i < num; i++) {
            size_t delimiter = recipe_positions[i];
            std::string result_link;
            delimiter += kSeventyNine;
            while (html_data_[delimiter] != '\"')
            {
                result_link.push_back(html_data_[delimiter]);
                delimiter++;
            }
            
            size_t title_delimiter = delimiter + 1;
            while (html_data_[title_delimiter] != '\"') {
                title_delimiter++;
            }
            title_delimiter++;
            std::string title;
            while (html_data_[title_delimiter] != '\"') {
                title.push_back(html_data_[title_delimiter]);
                title_delimiter++;
            }
            title = std::regex_replace(title, std::regex("&#x27;"), "\'");
            title = std::regex_replace(title, std::regex("&amp;"), "&");
            title = std::regex_replace(title, std::regex("&reg;"), " ");
            title = std::regex_replace(title, std::regex("&trade;"), "");
            results_titles_.push_back(title);
            results_.push_back(result_link);
        }
    }

    std::vector<std::string> Retriever::GetIngredients(std::string recipe_link) const{
        CurlObj data_recipe(recipe_link);
        std::string this_data = data_recipe.GetData();
        std::vector<std::string> supplies_return;
        std::string sub = "\"recipeIngredient\": [";
        size_t pos = this_data.find(sub, 0) + kTwentyOne;
        size_t end = this_data.find(']', pos);
        std::string ing;
        for (size_t i = pos; i < end; i++) {
            if (this_data[i] == '\n') {
                if (ing.length() == 0) {
                    continue;
                }
                ing = ing.substr(kEleven, ing.length() - kTwelve);
                ing = std::regex_replace(ing, std::regex(","), "");
                ing = std::regex_replace(ing, std::regex("\""), "");
                supplies_return.push_back(ing);
                ing.clear();
                continue;
            }
            ing.push_back(this_data[i]);
        }
        ing.clear();

        return supplies_return;
    }