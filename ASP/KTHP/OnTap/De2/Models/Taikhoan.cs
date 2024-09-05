namespace De2.Models
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    [Table("Taikhoan")]
    public partial class Taikhoan
    {
        public int id { get; set; }

        [StringLength(20)]
        public string tendn { get; set; }

        [StringLength(20)]
        public string matkhau { get; set; }
    }
}
