/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//  Written By : Group - 3a1w                    Environment : ubuntu:bionic               //
//  Date ......: December 3, 2021                   Compiler ...: clang-10                    //
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
//                              Framework/Set-up //
/////////////////////////////////////////////////////////////////////////////////////////////

#include "catch.hpp"
#include "../includes/retriever.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////
//                                 Includes //
/////////////////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <string>

/////////////////////////////////////////////////////////////////////////////////////////////
//                                Test Suite //
/////////////////////////////////////////////////////////////////////////////////////////////

/* Ensure correct link is assembled */
TEST_CASE("URL 1", "[URL1]") {
  std::vector<std::string> ingredients = {"tomato", "basil", "cheese"};
  Retriever retriever = Retriever(ingredients, 3);
  std::string solution = "https://www.allrecipes.com/search/results/?search=&IngIncl=tomato&IngIncl=basil&IngIncl=cheese";
  REQUIRE(retriever.GetLink() == solution);
}

/* Ensure correct link is assembled */
TEST_CASE("URL 2", "[URL2]") {
  std::vector<std::string> ingredients = {"potatoes", "oil", "garlic"};
  Retriever retriever = Retriever(ingredients, 3);
  std::string solution = "https://www.allrecipes.com/search/results/?search=&IngIncl=potatoes&IngIncl=oil&IngIncl=garlic";
  REQUIRE(retriever.GetLink() == solution);
}

/* Ensure correct link is assembled */
TEST_CASE("URL 3", "[URL3]") {
  std::vector<std::string> ingredients = {"eggs", "cheese", "sausage"};
  Retriever retriever = Retriever(ingredients, 3);
  std::string solution = "https://www.allrecipes.com/search/results/?search=&IngIncl=eggs&IngIncl=cheese&IngIncl=sausage";
  REQUIRE(retriever.GetLink() == solution);
}

/* Ensure correct number of links are retrieved */
TEST_CASE("NumLinks 1", "[NumLinks1]") {
  std::vector<std::string> ingredients = {"cheese", "bread"};
  Retriever retriever = Retriever(ingredients, 9);
  size_t num_results = retriever.GetNumOfResults();
  REQUIRE(retriever.GetLinks().size() == 9);
}

/* Ensure correct number of results are retrieved */
TEST_CASE("NumLinks 2", "[NumLinks2]") {
  std::vector<std::string> ingredients = {"milk", "sugar", "eggs"};
  Retriever retriever = Retriever(ingredients, 8);
  REQUIRE(retriever.GetLinks().size() == 8);
}

/* Ensure retrieved links are 0 for extraneous queries */
TEST_CASE("Exteraneous 1", "[Exteraneous1]") {
  std::vector<std::string> ingredients = {"car parts"};
  Retriever retriever = Retriever(ingredients, 8);
  REQUIRE(retriever.GetLinks().empty());
}

/* Ensure retrieved links are 0 for extraneous queries */
TEST_CASE("Exteraneous 2", "[Exteraneous2]") {
  std::vector<std::string> ingredients = {"steel mesh"};
  Retriever retriever = Retriever(ingredients, 5);
  REQUIRE(retriever.GetLinks().empty());
}

/* Ensure correct ingredients are retrieved */
TEST_CASE("Ingredients 1", "[Ingredients1]") {
  std::string link = "https://www.allrecipes.com/recipe/219460/grilled-garlic-rabbit/";
  std::vector<std::string> ingredients = {"rabbit"};
  Retriever retriever = Retriever(ingredients, 3);
  std::vector<std::string> solution = {"1 (3 pound) rabbit cleaned and cut into pieces", "1 cup salt-free garlic marinade (such as Mrs Dash®)"};
  REQUIRE(retriever.GetIngredients(link) == solution);
}

/* Ensure correct ingredients are retrieved */
TEST_CASE("Ingredients 2", "[Ingredients2]") { 
  std::string link = "https://www.allrecipes.com/recipe/257849/ritz-cream-cheese-veggie-swimmers/";
  std::vector<std::string> ingredients = {"cheese"};
  Retriever retriever = Retriever(ingredients, 3);
  std::vector<std::string> solution = {"24 RITZ Crackers", "½ cup cream cheese spread", "18 baby carrots cut into thin slices", "24 (1/4-inch) pimento slices"};
  REQUIRE(retriever.GetIngredients(link) == solution);
}

/* Ensure correct ingredients are retrieved */
TEST_CASE("Ingredients 3", "[Ingredients3]") { 
  std::string link = "https://www.allrecipes.com/recipe/258821/monster-munchies/";
  std::vector<std::string> ingredients = {"cereal"};
  Retriever retriever = Retriever(ingredients, 3);
  std::vector<std::string> solution = {"1 (10 ounce) package large marshmallows", "¼ cup chocolate-hazelnut spread (such as Nutella®)", "4 cups crispy rice cereal"};
  REQUIRE(retriever.GetIngredients(link) == solution);
}