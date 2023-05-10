#include <iostream>
#include <openssl/bn.h>
#include <openssl/rand.h>
#include <ctime>
#include <openssl/err.h>
#include <openssl/ec.h>
#include <openssl/obj_mac.h>


void print_bn(const char *label, BIGNUM *bn) {
    char *str = BN_bn2dec(bn);
    std::cout << label << ": " << str << std::endl;
    OPENSSL_free(str);
}


void print_ec_point(const EC_GROUP *group, const EC_POINT *point) {
    BN_CTX *ctx = BN_CTX_new();
    char *point_hex = EC_POINT_point2hex(group, point, POINT_CONVERSION_UNCOMPRESSED, ctx);
    printf("EC Point: %s\n", point_hex);
    OPENSSL_free(point_hex);
    BN_CTX_free(ctx);
}


int main() {
    // Initialize OpenSSL components

    EC_GROUP *group;
    EC_POINT *base, *result;
    BN_CTX *ctx;


    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    // Seed the random number generator
    srand(static_cast<unsigned int>(time(NULL)));
    RAND_poll();

    group = EC_GROUP_new_by_curve_name(NID_secp256k1);
    base = EC_POINT_new(group);
    result = EC_POINT_new(group);
    EC_POINT_copy(base, EC_GROUP_get0_generator(group));


    // Create BIGNUM context
    ctx = BN_CTX_new();

    // Create modulus 2^256
    BIGNUM *modulus = BN_new();
    BN_set_bit(modulus, 256);

    // Generate random BIGNUM
    BIGNUM *alpha = BN_new();
    BN_rand_range(alpha, modulus);

    BIGNUM *beta = BN_new();
    BN_rand_range(beta, modulus);

//    // Calculate the inverse modulo 2^256
//    BIGNUM *inverse = BN_mod_inverse(NULL, alpha, modulus, ctx);

    EC_POINT_mul(group, result, NULL, base, alpha, ctx);
    EC_POINT_mul(group, result, NULL, result, beta, ctx);

    print_ec_point(group, result);

    EC_POINT_mul(group, result, NULL, base, beta, ctx);
    EC_POINT_mul(group, result, NULL, result, alpha, ctx);

    print_ec_point(group, result);


    BN_free(alpha);
//    BN_free(inverse);
    BN_free(modulus);
    BN_CTX_free(ctx);
    EC_POINT_free(base);
    EC_POINT_free(result);
    EC_GROUP_free(group);
    // Clean up OpenSSL components
    EVP_cleanup();
    ERR_free_strings();

    return 0;
}
