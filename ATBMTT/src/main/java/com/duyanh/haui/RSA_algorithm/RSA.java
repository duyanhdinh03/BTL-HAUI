package com.duyanh.haui.RSA_algorithm;


import java.math.BigInteger;
import java.security.SecureRandom;

/**
 *
 * @author duyanh sdz
 */
public class RSA {

    private GenerateKey publicKey;
    private GenerateKey privateKey;

    public RSA(int numbits) {
        BigInteger p = BigInteger.probablePrime(numbits, new SecureRandom());
        BigInteger q = BigInteger.probablePrime(numbits, new SecureRandom());
        BigInteger n = p.multiply(q);
        BigInteger phi = p.subtract(BigInteger.ONE).multiply(q.subtract(BigInteger.ONE));
        BigInteger e, d;
        do {
            e = BigInteger.probablePrime(numbits, new SecureRandom());
        } while ((e.compareTo(BigInteger.ONE) == 1) && (e.compareTo(phi) == -1) && (e.gcd(phi).compareTo(BigInteger.ONE) != 0));
        d = e.modInverse(phi);
        publicKey = new GenerateKey(e, n);
        privateKey = new GenerateKey(d, n);
    }

    public RSA() {
    }

    public BigInteger encrypt(String msg) {
        return (new BigInteger(msg.getBytes())).modPow(publicKey.getComponent(), publicKey.getModulus());
    }

    public BigInteger decrypt(BigInteger encrypt_msg) {
        return encrypt_msg.modPow(privateKey.getComponent(), privateKey.getModulus());
    }

    public GenerateKey getPublicKey() {
        return publicKey;
    }

    public void setPublicKey(GenerateKey publicKey) {
        this.publicKey = publicKey;
    }

    public GenerateKey getPrivateKey() {
        return privateKey;
    }

    public void setPrivateKey(GenerateKey privateKey) {
        this.privateKey = privateKey;
    }
}
