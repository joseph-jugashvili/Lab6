
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>

#include <linux/ktime.h>
#include <linux/slab.h>

struct head_list {
  struct head_list *next;
  ktime_t time;
};

MODULE_AUTHOR("Andrukhovych Mykola IO-81");
MODULE_DESCRIPTION("Hello world printing Linux module for lab5");
MODULE_LICENSE("Dual BSD/GPL");

static struct head_list *head;

static int amount = 1;
module_param(amount, uint, S_IRUGO);
MODULE_PARM_DESC(amount, "Amount of outputting Hello worlds");



static int __init thisinit(void)
{
  uint i = 0;

  struct head_list *var_1, *var_2;

  head = kmalloc(sizeof(struct head_list *), GFP_KERNEL);

  var_1 = head;
  if (amount == 0) {
    printk(KERN_WARNING "Entered parameter equals 0");
  } else if (amount >= 5 && amount <= 10) {
    printk(KERN_WARNING "Entered parameter is between 5 and 10");
  } else if (amount > 10) {
    printk(KERN_ERR "Entered parameter is bigger than 10");
    return -EINVAL;
  }

  for (i = 0; i < amount; i++) {
    var_1->next = kmalloc(sizeof(struct head_list), GFP_KERNEL);
    var_1->time = ktime_get();
    pr_info("Hello World!");
    var_2 = var_1;
    var_1 = var_1->next;
  }
  if (amount != 0) {
    kfree(var_2->next);
    var_2->next = NULL;
  }

  printk(KERN_INFO "Amount: %d\n", amount);

  return 0;
}

static void __exit thisexit(void)
{
  struct head_list *var;

  while (head != NULL && amount != 0) {
    var = head;
    pr_info("Time: %lld", var->time);
    head = var->next;
    kfree(var);
  }
  pr_info("");
}

module_init(thisinit);
module_exit(thisexit);
