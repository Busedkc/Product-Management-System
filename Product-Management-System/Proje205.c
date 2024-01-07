#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define MAX_PRODUCTS 1000
#define MAX_NAME_LENGTH 50
#define FILENAME "products.csv"


// Define a structure to represent a product
struct Product {
    int id;
    char name[MAX_NAME_LENGTH];
    float price;
    int quantity;
};



// Function to check if a given product ID is already taken in the file
bool isProductIdTaken(FILE *fp, int newId) {
    struct Product product;
    
    // Check each product in the file
    rewind(fp);// Move the file position indicator to the beginning of the file
    while (fscanf(fp, "%d,%49[^,],%f,%d%*c", &product.id, product.name, &product.price, &product.quantity) != EOF) {
        if (product.id == newId) {
            // If the new ID has been used before, it is invalid
            return true;
        }
    }


     // If the new ID has not been used before, it is valid
    return false;
}


void addProduct(FILE *fp) {
    struct Product product;
    
    // Check the ID and prompt the user for input until a valid ID is provided
    do {
        printf("Enter Product ID: ");
        scanf("%d", &product.id);


        // Check if the ID is less than or equal to 100 or already taken
        if (product.id <= 100 || isProductIdTaken(fp, product.id)) {
            printf("Invalid ID! ID must be greater than 100 and not already taken. Please try again.\n");
        }


    } while (product.id <= 100 || isProductIdTaken(fp, product.id));


    // Prompt the user for product information
    printf("Enter Product Name: ");
    scanf("%s", product.name);
    printf("Enter Product Price: ");
    scanf("%f", &product.price);
    printf("Enter Product Quantity: ");
    scanf("%d", &product.quantity);


    // Write the product information to the file
    fprintf(fp, "%d,%s,%.2f,%d\n", product.id, product.name, product.price, product.quantity);
    printf("Product added successfully!\n");
  
    // Move the file position indicator to the end of the file
    fseek(fp, 0, SEEK_END);
}


void displayProducts(FILE *fp) {
    struct Product products[MAX_PRODUCTS];
    int count = 0;
    
    // Reset the file position indicator to the beginning of the file
    rewind(fp);


    // Read product information from the file into an array of Product structures
    while (fscanf(fp, "%d,%49[^,],%f,%d%*c", &products[count].id, products[count].name, &products[count].price, &products[count].quantity) != EOF) {
        count++;
 
         // Check if the maximum number of products has been reached
        if (count >= MAX_PRODUCTS) {
            printf("Too many products to display. Increase MAX_PRODUCTS if needed.\n");
            return;
        }
    }


    // Sorting process (Bubble Sort) based on product IDs
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (products[j].id > products[j + 1].id) {
                struct Product temp = products[j];
                products[j] = products[j + 1];
                products[j + 1] = temp;
            }
        }
    }


    // Header row
    printf("%-6s%-35s%-14s%-10s\n", "ID", "Name", "Price", "Quantity");


    // Data rows
    for (int i = 0; i < count; i++) {
        printf("%-6d%-35s%-14.2f%-10d\n", products[i].id, products[i].name, products[i].price, products[i].quantity);
    }
}








void searchProduct(FILE *fp) {
    int searchId, found = 0;
    struct Product product;


    // Prompt the user to enter the Product ID to search
    printf("Enter Product ID to search: ");
    scanf("%d", &searchId);




    // Reset the file position indicator to the beginning of the file
    rewind(fp);


    // Search for the product with the specified ID in the file
    while (fscanf(fp, "%d,%49[^,],%f,%d%*c", &product.id, product.name, &product.price, &product.quantity) != EOF) {
        if (product.id == searchId) {
            // Display product information if found
            printf("Product found!\n");
            printf("ID: %d, Name: %s, Price: %.2f, Quantity: %d\n",
                   product.id, product.name, product.price, product.quantity);
            found = 1;
            break;
        }
    }


    // Display a message if the product with the specified ID is not found
    if (!found) {
        printf("Product with ID %d not found.\n", searchId);
    }
}


void updateProduct(FILE *fp) {
    int updateId, found = 0;
    struct Product product;
    FILE *tempFile;
    // Create a temporary file to store updated product information
    tempFile = fopen("temp.csv", "w");


     // Prompt the user to enter the Product ID to update
    printf("Enter Product ID to update: ");
    scanf("%d", &updateId);
    
    // Reset the file position indicator to the beginning of the file
    rewind(fp);


    // Iterate through the file to find and update the product with the specified ID
    while (fscanf(fp, "%d,%49[^,],%f,%d%*c", &product.id, product.name, &product.price, &product.quantity) != EOF) {
        if (product.id == updateId) {
            found = 1;
            // Prompt the user for new product information
            printf("Enter new Product Name: ");
            scanf("%s", product.name);
            printf("Enter new Product Price: ");
            scanf("%f", &product.price);
            printf("Enter new Product Quantity: ");
            scanf("%d", &product.quantity);
        }
        // Write the product information to the temporary file
        fprintf(tempFile, "%d,%s,%.2f,%d\n", product.id, product.name, product.price, product.quantity);
    }


    // Close the files
    fclose(tempFile);
    fclose(fp);


    // Display a message if the product with the specified ID is not found
    if (!found) {
        remove("temp.csv");
        printf("Product with ID %d not found.\n", updateId);
        return;
    }


    // Remove the original file and rename the temporary file
    remove(FILENAME);
    rename("temp.csv", FILENAME);


    // Display a success message and reopen the original file in append mode
    printf("Product updated successfully!\n");
    fp = fopen(FILENAME, "a+");
}


void deleteProduct(FILE *fp) {
    int deleteId, found = 0;
    struct Product product;
    FILE *tempFile;


    // Create a temporary file to store product information after deletion
    tempFile = fopen("temp.csv", "w");


    // Prompt the user to enter the Product ID to delete
    printf("Enter Product ID to delete: ");
    scanf("%d", &deleteId);
   
    // Reset the file position indicator to the beginning of the file
    rewind(fp);


    // Iterate through the file to find and delete the product with the specified ID
    while (fscanf(fp, "%d,%49[^,],%f,%d%*c", &product.id, product.name, &product.price, &product.quantity) != EOF) {
        if (product.id == deleteId) {
            found = 1;
            continue;
        }


        // Write non-deleted products to the temporary file
        fprintf(tempFile, "%d,%s,%.2f,%d\n", product.id, product.name, product.price, product.quantity);
    }


    // Close the file
    fclose(tempFile);
    fclose(fp);


    // Display a message if the product with the specified ID is not found
    if (!found) {
        remove("temp.csv");
        printf("Product with ID %d not found.\n", deleteId);
        return;
    }


    // Remove the original file and rename the temporary file
    remove(FILENAME);
    rename("temp.csv", FILENAME);


    // Display a success message and reopen the original file in append mode
    printf("Product deleted successfully!\n");
    fp = fopen(FILENAME, "a+");
}


void filterProductsByPriceRange(FILE *fp) {
    float minPrice, maxPrice;
    struct Product product;
    int found = 0;


    // Prompt the user to enter the minimum and maximum price range
    printf("Enter the minimum price: ");
    scanf("%f", &minPrice);
    printf("Enter the maximum price: ");
    scanf("%f", &maxPrice);




    // Reset the file position indicator to the beginning of the file
    rewind(fp);


     // Display a header for the products within the specified price range
    printf("\nProducts within the price range %.2f to %.2f:\n", minPrice, maxPrice);
    printf("%-6s%-35s%-14s%-10s\n", "ID", "Name", "Price", "Quantity");


   // Iterate through the file and display products within the specified price range
    while (fscanf(fp, "%d,%49[^,],%f,%d%*c", &product.id, product.name, &product.price, &product.quantity) != EOF) {
        if (product.price >= minPrice && product.price <= maxPrice) {
            printf("%-6d%-35s%-14.2f%-10d\n", product.id, product.name, product.price, product.quantity);
            found = 1;
        }
    }
    
    // Display a message if no products are found within the specified price range
    if (!found) {
        printf("No products found within the specified price range.\n");
    }
}


void swap(struct Product *a, struct Product *b) {
// Swaps the content of two Product structures
     struct Product temp = *a;
     // Declares a temporary struct Product variable 'temp' and assigns the value of 'a' to it by dereferencing the pointer 'a'
     *a = *b;
     // Assigns the value of 'b' to 'a' by dereferencing both pointers, effectively swapping the contents of the structures 'a' and 'b'
     *b = temp;
     // Assigns the original value of 'a' (stored in 'temp') to 'b', completing the swap
 }




void sortByPriceAscending(struct Product *products, int size) {
    bool swapped;
// Bubble sort to arrange products by ascending price
    for (int i = 0; i < size - 1; i++) {
        swapped = false;
        for (int j = 0; j < size - i - 1; j++) {
            if (products[j].price > products[j + 1].price) {
            // If the prices are in the wrong order, swap the products
                swap(&products[j], &products[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) {
            // If no swapping occurred, the array is already sorted
            break;
        }
    }
}


void sortProductsByPriceAscending(FILE *fp) {
// Reads products from file, sorts them by ascending price, and displays them


    struct Product products[MAX_PRODUCTS];
    int count = 0;


    rewind(fp);
    // Positions the file pointer at the beginning of the file (assuming 'fp' is a file pointer)


    while (fscanf(fp, "%d,%49[^,],%f,%d%*c", &products[count].id, products[count].name, &products[count].price, &products[count].quantity) != EOF) {
        // Reads product information from the file using fscanf and stores it in the 'products' array
        count++;
        // Increments 'count' to keep track of the number of products read from the file
        if (count >= MAX_PRODUCTS) {
            printf("Too many products to sort. Increase MAX_PRODUCTS if needed.\n");
            return;
            // Checks if the number of products exceeds the defined maximum limit and returns if so
        }
    }


    sortByPriceAscending(products, count);
    // Calls a function 'sortByPriceAscending' to sort the 'products' array by price in ascending order


    printf("\nProducts sorted by price in ascending order:\n");
    printf("%-6s%-35s%-14s%-10s\n", "ID", "Name", "Price", "Quantity");
    // Prints headers for the product information in a formatted manner


    for (int i = 0; i < count; i++) {
        // Iterates through the sorted 'products' array to display product details
        printf("%-6d%-35s%-14.2f%-10d\n", products[i].id, products[i].name, products[i].price, products[i].quantity);
        // Prints each product's ID, name, price, and quantity in a formatted layout
    }
}




void sortByPriceDescending(struct Product *products, int size) {
    bool swapped;
   // Bubble sort to arrange products by descending price
    for (int i = 0; i < size - 1; i++) {
        swapped = false;
        for (int j = 0; j < size - i - 1; j++) {
            if (products[j].price < products[j + 1].price) {
            // If the prices are in the wrong order, swap the products
                swap(&products[j], &products[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) {
            // If no swapping occurred, the array is already sorted
            break;
        }
    }
}


void sortAndDisplayByPriceDescending(FILE *fp) {
// Reads products from file, sorts them by descending price, and displays them
    struct Product products[MAX_PRODUCTS];
    int count = 0;


    rewind(fp);
    // Positions the file pointer at the beginning of the file (assuming 'fp' is a file pointer)


    while (fscanf(fp, "%d,%49[^,],%f,%d%*c", &products[count].id, products[count].name, &products[count].price, &products[count].quantity) != EOF) {
        // Reads product information from the file using fscanf and stores it in the 'products' array
        count++;
        // Increments 'count' to keep track of the number of products read from the file
        if (count >= MAX_PRODUCTS) {
            printf("Too many products to sort. Increase MAX_PRODUCTS if needed.\n");
            return;
            // Checks if the number of products exceeds the defined maximum limit and returns if so
        }
    }


    sortByPriceDescending(products, count);
    // Calls a function 'sortByPriceDescending' to sort the 'products' array by price in descending order


    printf("\nProducts sorted by price in descending order:\n");
    printf("%-6s%-35s%-14s%-10s\n", "ID", "Name", "Price", "Quantity");
    // Prints headers for the product information in a formatted manner




    for (int i = 0; i < count; i++) {
        // Iterates through the sorted 'products' array to display product details
        printf("%-6d%-35s%-14.2f%-10d\n", products[i].id, products[i].name, products[i].price, products[i].quantity);
        // Prints each product's ID, name, price, and quantity in a formatted layout
    }
}


int main() { 
// Main menu to manage products


    FILE *fp;
    int choice;


    // Open or create the file for product management
    fp = fopen(FILENAME, "a+");


    // Main menu loop
    while (1) {
        printf("\n************************************\n");
        printf("******** Product Management ********\n");
        printf("************************************\n\n");
        printf("1. Add Product\n");
        printf("2. Display Products\n");
        printf("3. Search Product\n");
        printf("4. Update Product\n");
        printf("5. Delete Product\n");
        printf("6. Filter Products by Price Range\n");
        printf("7. Products Sorted by Price in Ascending Order\n");
        printf("8. Sort Products by Price Descending Order\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);


        switch (choice) {
            case 1:
               // Option to add a new product
                addProduct(fp);
                break;
            case 2:
                // Option to display all products
                printf("\n*************************************\n");
                printf("******** Displaying Products ********\n");
                printf("*************************************\n\n");
                rewind(fp);
                displayProducts(fp);
                break;
            case 3:
            // Option to search for a product
            printf("\n********************************\n");
                printf("******** Search Product ********\n");
                printf("********************************\n\n");
                searchProduct(fp);
                break;
                
            case 4:
                // Option to update a product
                 printf("\n********************************\n");
                printf("******** Update Product ********\n");
                printf("********************************\n\n");
                updateProduct(fp);
                break;
            
            case 5:
                // Option to delete a product
                printf("\n********************************\n");
                printf("******** Delete Product ********\n");
                printf("********************************\n\n");
                deleteProduct(fp);
                break;
           
                
            
            case 6:
         // Option to filter products by price range
                    printf("\n****************************************\n");
                     printf("**** Filter Products by Price Range ****\n");
                    printf("****************************************\n\n");
                   filterProductsByPriceRange(fp);
                     break;


            case 7:
        // Option to sort and display products by price in ascending order
                    printf("\n******************************************************\n");
                    printf("**** Sort and Display Products by Price Ascending ****\n");
                    printf("******************************************************\n\n");
                    sortProductsByPriceAscending(fp);
                    break;
            
            case 8:
         // Option to sort and display products by price in descending order
                    printf("\n*******************************************************\n");
                    printf("**** Sort and Display Products by Price Descending ****\n");
                    printf("*******************************************************\n\n");
                    sortAndDisplayByPriceDescending(fp);
                    break;


            
            case 9:
        // Option to exit the program
               printf("\n************************************\n");
               printf("********** Exiting Program **********\n");
               printf("************************************\n\n");
               fclose(fp);
               break;
            return 0;


            default:
          // Display a message for an invalid choice
                printf("Invalid choice. Please enter a valid choice.\n");
        }
    }
}