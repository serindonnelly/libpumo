'''
Created on 22 Nov 2013

@author: simon
'''
import scipy.stats as ss

def log_likelihood(x,mu,sigma):
  n = ss.norm(mu,sigma)
  return n.logpdf(x)

def log_likelihoods(xs,mus,sigmas):
  return map(lambda x: log_likelihood(*x),zip(xs,mus,sigmas))

def sum_log_likelihoods(xs,mus,sigmas):
  return sum(log_likelihoods(xs,mus,sigmas))

def likelihood_ratio(xs1,xs2,mus,sigmas):
  LL1 = sum_log_likelihoods(xs1,mus,sigmas)
  LL2 = sum_log_likelihoods(xs2, mus, sigmas)
  return -2*(LL1-LL2) #check this is correct