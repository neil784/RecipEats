#ifndef RETRIEVER_HPP
#define RETRIEVER_HPP

#include "../includes/CurlObj.hpp"
class Retriever {
public:
    Retriever() = default;
    Retriever(std::vector<std::string>& ingredients, size_t num_results);
    std::string GetLink() {return web_link_;};
    std::vector<std::string> GetIngredients(std::string recipe_link) const;
    std::vector<std::string> GetResultsTitles() const {return results_titles_;}
    std::vector<std::string> GetLinks() const {return results_;}
    size_t GetNumOfResults() const {return num_of_results_;}

private:
    void AssembleHtml();
    void TopResults(size_t num);

    std::string web_link_;
    std::vector<std::string> ingredients_;
    std::vector<std::string> results_titles_;
    std::vector<std::string> results_;
    std::string html_data_;
    size_t num_of_results_;

    const size_t kEleven = 11;
    const size_t kTwelve = 12;
    const size_t kTwentyOne = 21;
    const size_t kSeventyNine = 79;
};

#endif