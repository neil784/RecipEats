#include "../includes/retriever.hpp"
#include <fstream>
#include <chrono>
#include <ctime> 

/* Next Steps
  - Neaten up this file.  There is too much in here right now
  - Update my method for adding ingredients to the list so that it only adds unwanted ingredients
  - Make a separate file that keeps track of recipes I have already added
  - Reference the aforementioned file before I add a ingredients to list to see if the list already contains those ingriedients
  - Do the file above by links
  - Could use map, but that would only last for the lifetime of the program
  - General Formatting
  - Discuss Next Steps
  */


void HomeScreen();
void Quit();
void ReplaceFileLineDate(std::string path, int linetoerase);
void AddToShoppingList();
void PromptExpandIntoRecipe();
void ExpandIntoRecipe();
void PromptNumRecipes();
void OutputRecipes();

/* ANSI Color Guides */
  std::string red = "\033[1;31m";
  std::string white = "\033[;37m";
  std::string blue = "\033[1;34m";
  std::string green = "\033[1;32m";
  std::string yellow = "\033[1;33m";
  std::string end_color = "\033[0m";

/* Global Variables */
  std::vector<std::string> ingredients;
  size_t num_recipes_wanted;
  std::string user_input;
  Retriever* retriever;
  std::vector<std::string> titles;
  std::vector<std::string> link;
  size_t recipe_selected;
  size_t navigationchoice;
  bool recommended_recipes = false;

  /* Print out App name and directions of use */
  void PrintDirections() {
    system("clear");
    std::cout << "    - - - - - - - - - - -  " << yellow << "FIND NEW RECIPES" << end_color << " - - - - - - - - - - -"  << std::endl;
    std::cout << "========================================================================" << std::endl;
    std::cout << "Please enter your ingredients below, with each ingredient on a new line." << std::endl;
    std::cout << "  When finished entering ingredients, type \"search\" to find recipes!" << std::endl;
    std::cout << "========================================================================" << std::endl;
  }

  /* User inputs ingredients */
  void PromptUserForIngredients() {
    while (user_input != "search") {
    std::getline(std::cin, user_input);
    for (char& letter : user_input) {
      letter = (signed char) std::tolower(letter);
    }

    // Trims leading and trailing spaces
    user_input = std::regex_replace(user_input, std::regex("^ +| +$|( ) +"), "$1");
    // Adds %20 to necessary spaces (for html link)
    user_input = std::regex_replace(user_input, std::regex(" "), "%20");
    
    bool flag = false;
    for (const auto& something : ingredients) {
      if (something == user_input) {
        flag = true;
      }
    }
    if (!flag) {
      ingredients.push_back(user_input);
    }
  }
  // Erase the SEARCH command from ingredients vector
  ingredients.pop_back();

  // Check if no recipes are found
  Retriever check = Retriever(ingredients, 1);
  if (check.GetLinks().empty()) {
      system("clear");
      std::cout << white << "=============================================================================" << end_color << std::endl;
      std::cout << red << "Found 0 recipes!" << end_color << std::endl << std::endl;
      std::cout << "Check your spelling, try a more generic ingredient, or less ingredients" << std::endl;
      std::cout << white << "=============================================================================" << end_color << std::endl;
      std::cout << blue << "[1]" << end_color << " Search Again" << std::endl;
      std::cout << blue << "[2]" << end_color << " Return Home" << std::endl;
      std::cout << blue << "[3]" << end_color << " Quit" << std::endl;
      std::cout << white << "=============================================================================" << end_color << std::endl;
      std::cout << blue << "Select an option, and hit enter" << end_color << std::endl;
      std::getline(std::cin, user_input);
      while (true) {
        try {
          navigationchoice = std::stoi(user_input);
          if (navigationchoice > 0 && navigationchoice < 4) {
            break;
          }
          std::cout << red << "Please enter a valid option:" << end_color << std::endl;
          std::getline(std::cin, user_input);
        } catch (...) {
          std::cout << red << "Please enter a valid option:" << end_color << std::endl;
          std::getline(std::cin, user_input);
        }
      }
  if (navigationchoice == 1) {
    ingredients.clear();
    PrintDirections();
    PromptUserForIngredients();
    PromptNumRecipes();
    OutputRecipes();
    PromptExpandIntoRecipe();
    ExpandIntoRecipe();
  } else if (navigationchoice == 2) {
    HomeScreen();
  } else if (navigationchoice == 3) {
    Quit();
    }
  }
}

  /* Ask how many recipes the user wants */
  void PromptNumRecipes() {
    system("clear");
    std::cout << white << "=====================================" << end_color << std::endl;
    std::cout << yellow << "Enter the number of recipes you want:" << end_color << std::endl;
    std::cout << white << "=====================================" << end_color << std::endl;
    std::getline(std::cin, user_input);
    while (true) {
      try {
      num_recipes_wanted = std::stoi(user_input);
      break;
    } catch (...) {
      std::cout << red << "Please enter a valid number:" << end_color << std::endl;
      std::getline(std::cin, user_input);
      }
    }
    retriever = new Retriever(ingredients, num_recipes_wanted);
    titles = retriever->GetResultsTitles();
    link = retriever->GetLinks();
  }

  /* Helper function for OutputRecipes() */
  size_t FindMissingIngredients(const std::vector<std::string>& ingredients_in_recipe) {
      size_t to_return = 0;
      for (size_t i = 0; i < ingredients.size(); ++i) {
          std::string ingredient = ingredients.at(i);
          for (size_t j = 0; j < ingredients_in_recipe.size(); ++j) {
              std::string curr_ingredient = ingredients_in_recipe.at(j);
              if (curr_ingredient.find(ingredient) != std::string::npos) {
                  to_return++;
              }
          }
      }
      return ingredients_in_recipe.size() - to_return;
  }

  /* Outputs the found recipes */
  void OutputRecipes() {
    system("clear");
    std::cout << white << "=============================================================================" << end_color << std::endl;
    std::cout << green << "Found " << retriever->GetLinks().size() << " of " << retriever->GetNumOfResults() << " recipes!" << end_color << std::endl << std::endl;
    if (recommended_recipes) {
      for (size_t i = 0; i < retriever->GetLinks().size(); i++) {
         std::cout << green << "["<< i + 1 << "] " << end_color << retriever->GetResultsTitles().at(i) << std::endl;
      } 
    } else {
      for (size_t i = 0; i < retriever->GetLinks().size(); i++) {
         std::cout << green << "["<< i + 1 << "] " << end_color << retriever->GetResultsTitles().at(i) << " (missing " << red << FindMissingIngredients(retriever->GetIngredients(retriever->GetLinks().at(i))) << end_color << " ingredients)" << std::endl;
      } 
    }
    std::cout << white << "=============================================================================" << end_color << std::endl;
    std::cout << blue << "[0]" << end_color << " Return Home" << std::endl;
  }

  /* Prompts the user to expand into a recipe */
  void PromptExpandIntoRecipe() {
    std::cout << white << "=============================================================================" << end_color << std::endl;
    std::cout << blue << "Enter a recipe number to view more details or return home:" << end_color << std::endl;
    std::getline(std::cin, user_input);
    while (true) {
      try {
      recipe_selected = std::stoi(user_input);
      if (recipe_selected >= 0 && recipe_selected <= titles.size()) {
        break;
      } 
      std::cout << red << "Please select a valid recipe number:" << end_color << std::endl;
      std::getline(std::cin, user_input);
    } catch (...) {
      std::cout << red << "Please select a valid recipe number:" << end_color << std::endl;
      std::getline(std::cin, user_input);
      }
    }
    if (recipe_selected == 0) {
      HomeScreen();
    }
  }

  void ShoppingListScreen() {
    system("clear");
    std::string line;

    std::ifstream ifs("ShoppingList.txt", std::ios_base::in);
    int i = 0;
    while (getline(ifs, line)) {
      if (i < 3) {
        std::cout << blue << line << std::endl;
      } else {
        std::cout << end_color << line << std::endl;
      }
      i++;
    }
    if (i == 0) {
      std::cout << blue << "- - - - - - - - - - - SHOPPING LIST - - - - - - - - - - -" << std::endl;
      std::cout << "         Last Updated: No Ingredients Added Yet!" <<  std::endl;
      std::cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << std::endl;
      std::cout << end_color;
    }
    std::cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << std::endl;
    std::cout << end_color << "=========================================================" << std::endl;
    std::cout << blue << "Select an option, and hit enter" << end_color << std::endl;
    std::cout << "[1] Home" << std::endl;
    std::cout << "[2] Erase List" << std::endl;
    std::cout << "[3] Exit" << std::endl;
    std::cout << "=========================================================" << std::endl;
    std::getline(std::cin, user_input);
    while (true) {
      try {
        navigationchoice = std::stoi(user_input);
        if (navigationchoice > 0 && navigationchoice < 4) {
          break;
        }
        std::cout << red << "Please enter a valid option:" << end_color << std::endl;
        std::getline(std::cin, user_input);
      } catch (...) {
        std::cout << red << "Please enter a valid option:" << end_color << std::endl;
        std::getline(std::cin, user_input);
      }
    }

    if (navigationchoice == 1) {
      HomeScreen();
    } else if (navigationchoice == 2) {
      //implement
      std::ifstream fin;
      std::string listname = "ShoppingList.txt";
      fin.open(listname);
      std::ofstream temp;
      temp.open("temp.txt");
      temp << "- - - - - - - - - - - SHOPPING LIST - - - - - - - - - - -" << std::endl;
      temp << "         Last Updated: No Ingredients Added Yet!" << std::endl;
      temp << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << std::endl;
      const char * p = listname.c_str();
      remove(p);
      rename("temp.txt", p);
      ShoppingListScreen();
    } else if (navigationchoice == 3) {
      Quit();
    }
  }

  /* Expands into selected recipe */
  void ExpandIntoRecipe() {
    system("clear");
    std::cout << white << "=============================================================================" << end_color << std::endl;
    recipe_selected--;
    std::string recipe_link = link.at(recipe_selected);
    std::vector<std::string> recipe_ingredients = retriever->GetIngredients(recipe_link);
    std::cout << green << titles.at(recipe_selected) << std::endl << std::endl;
    std::cout << blue << link.at(recipe_selected) << std::endl << std::endl;
    for (auto& items : recipe_ingredients) {
      std::cout << white << items << end_color << std::endl;
    }
    std::cout << white << "=============================================================================" << end_color << std::endl;
    std::cout << blue << "[1]" << end_color << " Return Home" << std::endl;
    std::cout << blue << "[2]" << end_color << " Add ingredients to Shopping List" << std::endl;
    std::cout << blue << "[3]" << end_color << " View Shopping List" << std::endl;
    std::cout << blue << "[4]" << end_color << " Back" << std::endl;
    std::cout << blue << "[5]" << end_color << " Quit" << std::endl;
    std::cout << white << "=============================================================================" << end_color << std::endl;
    std::cout << yellow << "Select an option, and hit enter" << end_color << std::endl;

    std::getline(std::cin, user_input);
    while (true) {
      try {
        navigationchoice = std::stoi(user_input);
        if (navigationchoice > 0 && navigationchoice < 6) {
         break;
        }
        std::cout << red << "Please enter a valid option:" << end_color << std::endl;
        std::getline(std::cin, user_input);
      } catch (...) {
        std::cout << red << "Please enter a valid option:" << end_color << std::endl;
        std::getline(std::cin, user_input);
      }
    }
    if (navigationchoice == 1) {
      HomeScreen();
    } else if (navigationchoice == 2) {
      AddToShoppingList();
      recipe_selected++;
      ExpandIntoRecipe();
    } else if (navigationchoice == 3) {
      ShoppingListScreen();
    } else if (navigationchoice == 4) {
      OutputRecipes();
      PromptExpandIntoRecipe();
      ExpandIntoRecipe();
    } else if (navigationchoice == 5) {
      Quit();
    }
  }

  void HomeScreen() {
  system("clear");
  std::cout << green << "   RecipEats: A smarter way to plan your cooking!   " << end_color << std::endl;
  std::cout << "====================================================" << std::endl;
  std::cout << "- - - - - - - - - -  " << yellow << "HOMESCREEN" << end_color << "  - - - - - - - - - -" << std::endl;
  std::cout << blue << "[1]" << end_color << " Find Recipes by Ingredient" << std::endl;
  std::cout << blue << "[2]" << end_color << " See Shopping List" << std::endl;
  std::cout << blue << "[3]" << end_color << " See Recommended Recipes" << std::endl;
  std::cout << blue << "[4]" << end_color << " Quit" << std::endl;
  std::cout << "====================================================" << std::endl;
  std::cout << blue << "Select an option, and hit enter" << end_color << std::endl;

  std::getline(std::cin, user_input);
  while (true) {
    try {
      navigationchoice = std::stoi(user_input);
      if (navigationchoice > 0 && navigationchoice < 5) {
        break;
      }
      std::cout << red << "Please enter a valid option:" << end_color << std::endl;
      std::getline(std::cin, user_input);
    } catch (...) {
      std::cout << red << "Please enter a valid option:" << end_color << std::endl;
      std::getline(std::cin, user_input);
    }
  }
  
  if (navigationchoice == 1) {
    // function for Find Recipes
    ingredients.clear();
    recommended_recipes = false;
    PrintDirections();
    PromptUserForIngredients();
    PromptNumRecipes();
    OutputRecipes();
    PromptExpandIntoRecipe();
    ExpandIntoRecipe();
  } 
  if (navigationchoice == 2) {
    // function for shopping list, Antony (Me), will write this
    ShoppingListScreen();
  } 
  if (navigationchoice == 3) {
    // recommended recipes
    ingredients.clear();
    recommended_recipes = true;
    PromptNumRecipes();
    OutputRecipes();
    PromptExpandIntoRecipe();
    ExpandIntoRecipe();
  } 
  if (navigationchoice == 4) {
    Quit();
  } 
}

void AddToShoppingList() {

  ReplaceFileLineDate("ShoppingList.txt", 1);

  std::fstream f("ShoppingList.txt", f.out | f.app);
  // f.open("/home/vagrant/src/final-project-3a1w/ShoppingList.txt", std::ios_base::app);
  if (!f.is_open()) {
    throw std::runtime_error("File did not open!");
  }

  std::string recipe_link = link.at(recipe_selected);
  std::vector<std::string> recipe_ingredients = retriever->GetIngredients(recipe_link);

  for (auto& items : recipe_ingredients) {
      std::cout << items << std::endl; 
      f << items << std::endl;
  }
  f.close();
  std::fstream titlecheck{"ShoppingList.txt"};
  std::string line;
  getline(titlecheck, line);
  if (line != "- - - - - - - - - - - SHOPPING LIST - - - - - - - - - - -") {
    titlecheck.close();
    std::string line;
    std::ifstream fin;
    fin.open("ShoppingList.txt");
    std::ofstream temp;
    temp.open("temp.txt");
    temp << "- - - - - - - - - - - SHOPPING LIST - - - - - - - - - - -" << std::endl;
    temp << "         Last Updated: No Ingredients Added Yet!" << std::endl;
    temp << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << std::endl;
    while (getline(fin, line)) {
      temp << line << std::endl;
    }
    std::string listname = "ShoppingList.txt";
    const char * p = listname.c_str();
    remove(p);
    rename("temp.txt", p);
    ReplaceFileLineDate(listname, 1);
  }
}

void Quit() {
  system("clear");
  std::cout << "  - - - - - " << yellow << "Thank you for using RecipEats!" << end_color << " - - - - -" << std::endl;
  std::cout << "======================================================" << std::endl;
  std::cout << "Created by: Art Suthigoseeya, Antony Silvetti-Schmitt," << std::endl;
  std::cout << "          Joseph Ku, and Neil Prabhu" << std::endl;
  std::cout << "======================================================" << std::endl;
  std::cout << "   This project was made possible by AllRecipes.com   " << std::endl;
  std::cout << "======================================================" << std::endl;
  exit(0);
}

void ReplaceFileLineDate(std::string path, int linetoerase) {
    std::string line;
    std::ifstream fin;
    
    fin.open(path);
    // contents of path must be copied to a temp file then
    // renamed back to the path file
    std::ofstream temp;
    temp.open("temp.txt");
    int i = 0;
    while (getline(fin,line)) {
      if (i == linetoerase) {
        auto now = std::chrono::system_clock::now();
        std::time_t nowtime = std::chrono::system_clock::to_time_t(now);
        std::time_t* nowtimept = &nowtime;
        temp << "         Last Updated: " << std::ctime(nowtimept);
      } else {
        temp << line << std::endl;
      }
      i++;
    }
    temp.close();
    fin.close();

    // required conversion for remove and rename functions
    const char * p = path.c_str();
    remove(p);
    rename("temp.txt", p);
}

int main() {
  HomeScreen();
}
